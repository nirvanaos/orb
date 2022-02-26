/// \file
/*
* Nirvana IDL support library.
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
#ifndef NIRVANA_ORB_INTERFACE_H_
#define NIRVANA_ORB_INTERFACE_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include "primitive_types.h"
#include "StringBase.h"

namespace CORBA {
namespace Internal {

template <class I> class I_ptr;
template <class I> class I_ref;
#ifdef LEGACY_CORBA_CPP
template <class I> class I_var;
#endif
class Interface;
template <> class I_ptr <Interface>;
template <> class I_ref <Interface>;

/// Base for all interface ABIs.
/// Provides life-cycle management and interface identification.
class Interface
{
	Interface (const Interface&) = delete;

public:
	/// Entry point vector
	struct EPV
	{
		const Char* interface_id;
		Interface* (*duplicate) (Interface* obj, Interface* environment);
		void (*release) (Interface*);
	};

	Interface (const EPV& epv) NIRVANA_NOEXCEPT :
		_epv_ref (epv)
	{}

	const EPV& _epv () const NIRVANA_NOEXCEPT
	{
		return _epv_ref;
	}

	Interface& operator = (const Interface&) NIRVANA_NOEXCEPT
	{
		return *this;	// Do nothing
	}

	static Interface* _check (Interface* obj, String_in interface_id);

	typedef I_ptr <Interface> _ptr_type;
#ifdef LEGACY_CORBA_CPP
	typedef I_var <Interface> _var_type;
#else
	typedef I_ref <Interface> _ref_type;
#endif
	static I_ptr <Interface> _nil () NIRVANA_NOEXCEPT;

private:
	const EPV& _epv_ref;
};

Interface* interface_duplicate (Interface* obj);
void interface_release (Interface* itf) NIRVANA_NOEXCEPT;

}

class Environment;

}

#endif
