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
#ifndef NIRVANA_ORB_SOFTFLOAT_IMPL_H_
#define NIRVANA_ORB_SOFTFLOAT_IMPL_H_
#pragma once

#include "SoftFloat.h"
#include "TypeFixLen.h"
#include <Nirvana/platform.h>

namespace CORBA {
namespace Internal {

template <size_t size, size_t align>
struct alignas (align) FloatBits
{
	uint8_t bits [size];
};

typedef FloatBits <sizeof (float), alignof (float)> NativeFloat;
typedef FloatBits <sizeof (double), alignof (double)> NativeDouble;
typedef std::conditional <Nirvana::PLATFORM == Nirvana::PLATFORM_I386 || Nirvana::PLATFORM == Nirvana::PLATFORM_X64, FloatBits <10, 8>,
	FloatBits <sizeof (long double), alignof (long double)> >::type NativeLongDouble;

template <size_t size, size_t align>
struct Type <FloatBits <size, align> > : public TypeFixLen <FloatBits <size, align>, false>
{};

template <>
struct Type <FloatIEEE16> : public TypeFixLen <FloatIEEE16, false>
{};

}
}

#include "SFloat.h"

namespace CORBA {
namespace Internal {

inline
SoftFloat <16>::SoftFloat (const long double& val)
{
	if (sizeof (long double) == sizeof (NativeLongDouble))
		g_sfloat16->from_long_double (reinterpret_cast <const NativeLongDouble&> (val), bits_);
	else {
		// MSVC
		static_assert (sizeof (long double) == sizeof (double), "sizeof (long double) == sizeof (double)");
		g_sfloat16->from_double (reinterpret_cast <const NativeDouble&> (val), bits_);
	}
}

inline
SoftFloat <16>::SoftFloat (const int32_t& val)
{
	g_sfloat16->from_long (val, bits_);
}

inline
SoftFloat <16>::SoftFloat (const uint32_t& val)
{
	g_sfloat16->from_unsigned_long (val, bits_);
}

inline
SoftFloat <16>::SoftFloat (const int64_t& val)
{
	g_sfloat16->from_long_long (val, bits_);
}

inline
SoftFloat <16>::SoftFloat (const uint64_t& val)
{
	g_sfloat16->from_unsigned_long_long (val, bits_);
}

inline
SoftFloat <16>::operator double () const
{
	double ret;
	g_sfloat16->to_double (bits_, reinterpret_cast <NativeDouble&> (ret));
	return ret;
}

inline
SoftFloat <16>::operator long double () const
{
	if (sizeof (long double) == sizeof (NativeLongDouble)) {
		long double ret;
		g_sfloat16->to_long_double (bits_, reinterpret_cast <NativeLongDouble&> (ret));
		return ret;
	} else {
		// MSVC
		static_assert (sizeof (long double) == sizeof (double), "sizeof (long double) == sizeof (double)");
		double ret;
		g_sfloat16->to_double (bits_, reinterpret_cast <NativeDouble&> (ret));
		return ret;
	}
}

inline
SoftFloat <16>::operator int32_t () const
{
	int32_t ret;
	g_sfloat16->to_long (bits_, ret);
	return ret;
}

inline
SoftFloat <16>::operator int64_t () const
{
	int64_t ret;
	g_sfloat16->to_long_long (bits_, ret);
	return ret;
}

}
}

#endif
