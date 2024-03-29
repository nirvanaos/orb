/// \file
/// \brief CORBA basic data types.
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
#ifndef NIRVANA_ORB_BASIC_TYPES_H_
#define NIRVANA_ORB_BASIC_TYPES_H_
#pragma once

#include "TypePrimitive.h"
#include "tc_constants.h"
#include "StaticTC.h"

#define NIRVANA_BASIC_TYPE(T, tc) template <> struct Type <T> : TypePrimitive <T> {\
static I_ptr <TypeCode> type_code () { return tc; } };

namespace CORBA {
namespace Internal {

NIRVANA_BASIC_TYPE (Boolean, _tc_boolean)
NIRVANA_BASIC_TYPE (Char, _tc_char)
NIRVANA_BASIC_TYPE (WChar, _tc_wchar)
NIRVANA_BASIC_TYPE (Octet, _tc_octet)
NIRVANA_BASIC_TYPE (Short, _tc_short)
NIRVANA_BASIC_TYPE (UShort, _tc_ushort)
NIRVANA_BASIC_TYPE (Long, _tc_long)
NIRVANA_BASIC_TYPE (LongLong, _tc_longlong)
NIRVANA_BASIC_TYPE (ULong, _tc_ulong)
NIRVANA_BASIC_TYPE (ULongLong, _tc_ulonglong)
NIRVANA_BASIC_TYPE (Float, _tc_float)
NIRVANA_BASIC_TYPE (Double, _tc_double)
NIRVANA_BASIC_TYPE (LongDouble, _tc_longdouble)

}
}

#undef NIRVANA_BASIC_TYPE
#define NIRVANA_BASIC_TYPE(T) template <> struct traits <CORBA::T> {\
	using value_type = CORBA::T;\
	using in_type = CORBA::T;\
	using out_type = CORBA::T&;\
	using inout_type = CORBA::T&;\
};

namespace IDL {

NIRVANA_BASIC_TYPE (Boolean)
NIRVANA_BASIC_TYPE (Char)
NIRVANA_BASIC_TYPE (WChar)
NIRVANA_BASIC_TYPE (Octet)
NIRVANA_BASIC_TYPE (Short)
NIRVANA_BASIC_TYPE (UShort)
NIRVANA_BASIC_TYPE (Long)
NIRVANA_BASIC_TYPE (LongLong)
NIRVANA_BASIC_TYPE (ULong)
NIRVANA_BASIC_TYPE (ULongLong)
NIRVANA_BASIC_TYPE (Float)
NIRVANA_BASIC_TYPE (Double)
NIRVANA_BASIC_TYPE (LongDouble)

}

#undef NIRVANA_BASIC_TYPE

#endif
