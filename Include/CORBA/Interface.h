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
#include "StringView.h"

namespace CORBA {
namespace Internal {

template <class Itf> class I_ptr;
template <class Itf> class I_ref;
#ifdef LEGACY_CORBA_CPP
template <class Itf> class I_var;
#endif
class Interface;
template <> class I_ptr <Interface>;
template <> class I_ref <Interface>;

/// Base for all interface ABIs.
/// Provides life-cycle management and interface identification.
class Interface
{
public:
	/// Entry point vector
	struct EPV
	{
		const Char* interface_id;
		Interface* (*duplicate) (Interface* obj, Interface* environment);
		void (*release) (Interface*);
	};

	constexpr Interface (const EPV& epv) noexcept :
		_epv_ref (epv)
	{}

	Interface (const Interface&) = default;

	const EPV& _epv () const noexcept
	{
		return _epv_ref;
	}

	Interface& operator = (const Interface&) noexcept
	{
		return *this;	// Do nothing
	}

	Interface& operator = (const Interface&&) noexcept
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
	static I_ptr <Interface> _nil () noexcept;

	static Interface* _unsafe_cast (Interface* p) noexcept
	{
		return p;
	}

private:
	const EPV& _epv_ref;
};

Interface* interface_duplicate (Interface* obj);
void interface_release (Interface* itf) noexcept;

}

class Environment;

}

#endif
