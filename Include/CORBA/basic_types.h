/// \file basic_types.h
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

#include <stdint.h>
#include "Boolean.h"
#include "TypeFixLen.h"

#define DECLARE_BASIC_TYPE(T) namespace Nirvana { template <> struct Nirvana::Type <T> : Nirvana::TypeFixLen <T> {}; }\
typedef Nirvana::Type <T>::C_out T##_out; typedef Nirvana::Type <T>::C_inout T##_inout;

namespace CORBA {

// Primitive types

typedef char Char;
DECLARE_BASIC_TYPE (Char)

typedef wchar_t WChar;
DECLARE_BASIC_TYPE (WChar)

typedef uint8_t Octet;
DECLARE_BASIC_TYPE (Octet)

typedef int16_t Short;
DECLARE_BASIC_TYPE (Short)

typedef uint16_t UShort;
DECLARE_BASIC_TYPE (UShort)

typedef int32_t Long;
DECLARE_BASIC_TYPE (Long)

typedef int64_t LongLong;
DECLARE_BASIC_TYPE (LongLong)

typedef uint32_t ULong;
DECLARE_BASIC_TYPE (ULong)

typedef uint64_t ULongLong;
DECLARE_BASIC_TYPE (ULongLong)

typedef float Float;
DECLARE_BASIC_TYPE (Float)

typedef double Double;
DECLARE_BASIC_TYPE (Double)

typedef long double LongDouble;
DECLARE_BASIC_TYPE (LongDouble)

typedef Long Flags;
  
} // namespace CORBA

#undef DECLARE_BASIC_TYPE

#endif
