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
#ifndef NIRVANA_ORB_TYPE_H_
#define NIRVANA_ORB_TYPE_H_
#pragma once

#include "TypeFixLen.inl"
#include "TypeVarLen.inl"
#include "TypePrimitive.inl"
#include "TypeEnum.inl"
#include "Type_interface.inl"
#include "Object.inl"
#include "ValueBase.inl"
#include "ValueBox.h"
#include "AbstractBase.inl"
#include "Proxy/TypeCodeString.h"
#include "Proxy/TypeCodeSequence.h"
#include "Proxy/TypeCodeArray.h"
#include "Proxy/TypeCodeFixed.h"

namespace CORBA {
namespace Internal {

/// The `void` type.
template <>
struct Type <void>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_void;
	}
};

}
}

#ifndef LEGACY_CORBA_CPP

namespace IDL {

// IDL::traits
template <class I>
struct traits
{
	typedef CORBA::Internal::I_ref <I> ref_type;
	typedef CORBA::Internal::I_ptr <I> ptr_type;
};

}

#endif

#endif
