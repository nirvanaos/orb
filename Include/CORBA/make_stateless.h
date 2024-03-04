/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_MAKE_STATELESS_H_
#define NIRVANA_ORB_MAKE_STATELESS_H_
#pragma once

#include "core_objects.h"

#ifndef NIRVANA_PROCESS

#include "ObjectFactoryInc.h"
#include "servant_reference.h"
#include <utility>

namespace CORBA {

/// Detect if we are in the free sync context.
inline bool is_free_sync_context ()
{
	return Internal::object_factory->is_free_sync_context ();
}

/// \brief Create stateless servant.
/// 
/// Stateless servant lives in the free synchronization context.
/// So operations may be called in parallel, without a synchronization.
/// Stateless servant must not change it's state during the all life cycle.
/// Usually such servant resides in the read-only memory, so any change of the
/// object variables will cause access violation exception.
/// 
/// Note that destructor also have read-only access to object and can not change
/// object variables during the object destruction. Generally it means that we can not
/// use portable (virtual) implementation, because it changes vtable pointers during
/// the object destruction,
/// 
/// \tparam T A servant type.
/// \tparam Args Constructor parameter types.
/// \param args Servant constructor parameters.
/// \returns Servant reference.
/// 
template <class T, class ... Args>
servant_reference <T> make_stateless (Args ... args)
{
	// Allocate temporary space for servant in stack
	typename std::aligned_storage <sizeof (T), alignof (T)>::type tmp;

	// Create StatelessCreationFrame and register it with ObjectFactory
	Internal::ObjectFactory::StatelessCreationFrame scf;
	scf.tmp (&tmp);
	scf.size (sizeof (T));
	Internal::object_factory->stateless_begin (scf);

	T* tmp_serv = nullptr;
	try {
		// Create servant
		T* tmp_serv = new (&tmp) T (std::forward <Args> (args)...);

		// Move servant to the stateless memory and return reference to it.
		return servant_reference <T> ((T*)Internal::object_factory->stateless_end (true), false);
	} catch (...) {
		// Destroy servant
		if (tmp_serv)
			tmp_serv->~T ();
		
		// Inform ObjectFactory that servant creation was failed
		Internal::object_factory->stateless_end (false);
		throw;
	}
}

}

#endif
#endif
