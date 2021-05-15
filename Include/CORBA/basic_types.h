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

#ifdef LEGACY_CORBA_CPP
#define DECLARE_TYPE_OUT(T) typedef T& T##_out;
#else
#define DECLARE_TYPE_OUT(T)
#endif

#define DECLARE_BASIC_TYPE(T) namespace Internal { template <> struct Internal::Type <T> : Internal::TypeFixLen <T> {\
static I_ptr <TypeCode> type_code (); }; }\
DECLARE_TYPE_OUT(T)

namespace CORBA {

DECLARE_BASIC_TYPE (Char)
DECLARE_BASIC_TYPE (WChar)
DECLARE_BASIC_TYPE (Octet)
DECLARE_BASIC_TYPE (Short)
DECLARE_BASIC_TYPE (UShort)
DECLARE_BASIC_TYPE (Long)
DECLARE_BASIC_TYPE (LongLong)
DECLARE_BASIC_TYPE (ULong)
DECLARE_BASIC_TYPE (ULongLong)
DECLARE_BASIC_TYPE (Float)
DECLARE_BASIC_TYPE (Double)
DECLARE_BASIC_TYPE (LongDouble)

namespace Internal {
/// The `void` type.
template <>
struct Type <void>
{
	static I_ptr <TypeCode> type_code ();
};

}
}

#undef DECLARE_BASIC_TYPE

#endif
