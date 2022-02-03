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
#pragma once

#include "float_types.h"
#include <stdint.h>

namespace CORBA {

// Primitive types

typedef bool Boolean;
typedef char Char;
typedef wchar_t WChar;
/* TODO:
/// On systems with 32-bit or biggest word, WChar is 32 bit.
/// On 16-bit systems WChar is 16 bit and only UCS-16 encoding is supported.
typedef std::conditional <sizeof (size_t) >= 4,
	std::conditional <sizeof (wchar_t) == 4, wchar_t, char32_t>::type,
	std::conditional <sizeof (wchar_t) == 2, wchar_t, char16_t>::type
>::type	WChar;
*/

typedef uint8_t Octet;
typedef int16_t Short;
typedef uint16_t UShort;
typedef int32_t Long;
typedef int64_t LongLong;
typedef uint32_t ULong;
typedef uint64_t ULongLong;
typedef Long Flags;

// Boolean constant
#ifdef FALSE
#undef FALSE
#endif
  
#ifdef TRUE
#undef TRUE
#endif

const bool TRUE = true;
const bool FALSE = false;

}

#endif
