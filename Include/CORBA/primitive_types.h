/// \file
/// \brief CORBA primitive data types.
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
#ifndef NIRVANA_ORB_PRIMITIVE_TYPES_H_
#define NIRVANA_ORB_PRIMITIVE_TYPES_H_

#include <stdint.h>

namespace CORBA {

// Primitive types

typedef bool Boolean;
typedef char Char;
typedef wchar_t WChar;
typedef uint8_t Octet;
typedef int16_t Short;
typedef uint16_t UShort;
typedef int32_t Long;
typedef int64_t LongLong;
typedef uint32_t ULong;
typedef uint64_t ULongLong;
typedef float Float;
typedef double Double;
typedef long double LongDouble;
typedef Long Flags;
  
}

#endif
