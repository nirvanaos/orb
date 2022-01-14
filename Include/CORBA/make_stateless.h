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
#include "ObjectFactoryInc.h"
#include "servant_reference.h"
#include <utility>

namespace CORBA {

template <class T, class ... Args>
servant_reference <T> make_stateless (Args ... args)
{
	typename std::aligned_storage <sizeof (T), alignof (T)>::type tmp;
	Internal::ObjectFactory::StatelessCreationFrame scb (&tmp, sizeof (T), 0);
	Internal::g_object_factory->stateless_begin (scb);
	try {
		new (&tmp) T (std::forward <Args> (args)...);
		return servant_reference <T> ((T*)Internal::g_object_factory->stateless_end (true), false);
	} catch (...) {
		Internal::g_object_factory->stateless_end (false);
		throw;
	}
}

}

#endif
