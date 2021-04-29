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

#include "Boolean.h"
#include "TypeFixLen.h"
#include "tc_constants.h"

#ifdef LEGACY_CORBA_CPP
#define DECLARE_TYPE_OUT(T) typedef T& T##_out;
#else
#define DECLARE_TYPE_OUT(T)
#endif

#define DECLARE_BASIC_TYPE(T, tc) namespace Nirvana { template <> struct Nirvana::Type <T> : Nirvana::TypeFixLen <T> {\
static I_ptr <TypeCode> type_code () { return tc; } }; }\
DECLARE_TYPE_OUT(T)

namespace CORBA {

DECLARE_BASIC_TYPE (Char, _tc_char)
DECLARE_BASIC_TYPE (WChar, _tc_wchar)
DECLARE_BASIC_TYPE (Octet, _tc_octet)
DECLARE_BASIC_TYPE (Short, _tc_short)
DECLARE_BASIC_TYPE (UShort, _tc_ushort)
DECLARE_BASIC_TYPE (Long, _tc_long)
DECLARE_BASIC_TYPE (LongLong, _tc_longlong)
DECLARE_BASIC_TYPE (ULong, _tc_ulong)
DECLARE_BASIC_TYPE (ULongLong, _tc_ulonglong)
DECLARE_BASIC_TYPE (Float, _tc_float)
DECLARE_BASIC_TYPE (Double, _tc_double)
DECLARE_BASIC_TYPE (LongDouble, _tc_longdouble)

namespace Nirvana {

inline
I_ptr <TypeCode> Type <Boolean>::type_code ()
{
	return _tc_boolean;
}

}

}

#undef DECLARE_BASIC_TYPE

#endif
