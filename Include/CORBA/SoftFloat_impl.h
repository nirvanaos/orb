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

template <size_t size>
struct Type <FloatIEEE <size> > : public TypeFixLen <FloatIEEE <size>, false>
{};

typedef FloatIEEE <4> FloatIEEE4;
typedef FloatIEEE <8> FloatIEEE8;
typedef FloatIEEE <16> FloatIEEE16;

}
}

#include "SFloat.h"

namespace CORBA {
namespace Internal {

void set_softfloat_exception_flags (unsigned f) noexcept;

// 32 bit

inline
SoftFloat <4>::SoftFloat (const float& val)
{
	set_softfloat_exception_flags (sfloat_4->from_float (reinterpret_cast <const NativeFloat&> (val), bits_));
}

inline
SoftFloat <4>::SoftFloat (const int32_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_4->from_long (val, bits_));
}

inline
SoftFloat <4>::SoftFloat (const uint32_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_4->from_unsigned_long (val, bits_));
}

inline
SoftFloat <4>::SoftFloat (const int64_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_4->from_long_long (val, bits_));
}

inline
SoftFloat <4>::SoftFloat (const uint64_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_4->from_unsigned_long_long (val, bits_));
}

inline
SoftFloat <4>::operator float () const
{
	float ret;
	set_softfloat_exception_flags (sfloat_4->to_float (bits_, reinterpret_cast <NativeFloat&> (ret)));
	return ret;
}

inline
SoftFloat <4>::operator int32_t () const
{
	int32_t ret;
	set_softfloat_exception_flags (sfloat_4->to_long (bits_, ret));
	return ret;
}

inline
SoftFloat <4>::operator int64_t () const
{
	int64_t ret;
	set_softfloat_exception_flags (sfloat_4->to_long_long (bits_, ret));
	return ret;
}

inline
SoftFloat <4>& SoftFloat <4>::operator += (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_4->add (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <4>& SoftFloat <4>::operator -= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_4->sub (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <4>& SoftFloat <4>::operator *= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_4->mul (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <4>& SoftFloat <4>::operator /= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_4->div (bits_, rhs.bits_));
	return *this;
}

inline
bool SoftFloat <4>::eq (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_4->eq (bits_, rhs.bits_, ret));
	return ret;
}

inline
bool SoftFloat <4>::le (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_4->le (bits_, rhs.bits_, ret));
	return ret;
}

inline
bool SoftFloat <4>::lt (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_4->lt (bits_, rhs.bits_, ret));
	return ret;
}

// 64 bit

inline
SoftFloat <8>::SoftFloat (const double& val)
{
	set_softfloat_exception_flags (sfloat_8->from_double (reinterpret_cast <const NativeDouble&> (val), bits_));
}

inline
SoftFloat <8>::SoftFloat (const int32_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_8->from_long (val, bits_));
}

inline
SoftFloat <8>::SoftFloat (const uint32_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_8->from_unsigned_long (val, bits_));
}

inline
SoftFloat <8>::SoftFloat (const int64_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_8->from_long_long (val, bits_));
}

inline
SoftFloat <8>::SoftFloat (const uint64_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_8->from_unsigned_long_long (val, bits_));
}

inline
SoftFloat <8>::operator double () const
{
	double ret;
	set_softfloat_exception_flags (sfloat_8->to_double (bits_, reinterpret_cast <NativeDouble&> (ret)));
	return ret;
}

inline
SoftFloat <8>::operator int32_t () const
{
	int32_t ret;
	set_softfloat_exception_flags (sfloat_8->to_long (bits_, ret));
	return ret;
}

inline
SoftFloat <8>::operator int64_t () const
{
	int64_t ret;
	set_softfloat_exception_flags (sfloat_8->to_long_long (bits_, ret));
	return ret;
}

inline
SoftFloat <8>& SoftFloat <8>::operator += (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_8->add (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <8>& SoftFloat <8>::operator -= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_8->sub (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <8>& SoftFloat <8>::operator *= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_8->mul (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <8>& SoftFloat <8>::operator /= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_8->div (bits_, rhs.bits_));
	return *this;
}


inline
bool SoftFloat <8>::eq (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_8->eq (bits_, rhs.bits_, ret));
	return ret;
}

inline
bool SoftFloat <8>::le (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_8->le (bits_, rhs.bits_, ret));
	return ret;
}

inline
bool SoftFloat <8>::lt (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_8->lt (bits_, rhs.bits_, ret));
	return ret;
}

// 128 bit

inline
SoftFloat <16>::SoftFloat (const double& val)
{
	set_softfloat_exception_flags (sfloat_16->from_double (reinterpret_cast <const NativeDouble&> (val), bits_));
}

inline
SoftFloat <16>::SoftFloat (const long double& val)
{
	if (sizeof (long double) == sizeof (NativeLongDouble))
		set_softfloat_exception_flags (sfloat_16->from_long_double (reinterpret_cast <const NativeLongDouble&> (val), bits_));
	else {
		// MSVC
		assert (sizeof (long double) == sizeof (double));
		set_softfloat_exception_flags (sfloat_16->from_double (reinterpret_cast <const NativeDouble&> (val), bits_));
	}
}

inline
SoftFloat <16>::SoftFloat (const int32_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_16->from_long (val, bits_));
}

inline
SoftFloat <16>::SoftFloat (const uint32_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_16->from_unsigned_long (val, bits_));
}

inline
SoftFloat <16>::SoftFloat (const int64_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_16->from_long_long (val, bits_));
}

inline
SoftFloat <16>::SoftFloat (const uint64_t& val)
{
	if (val == 0) {
		bits_.zero ();
		set_softfloat_exception_flags (0);
	} else
		set_softfloat_exception_flags (sfloat_16->from_unsigned_long_long (val, bits_));
}

inline
SoftFloat <16>::operator long double () const
{
	long double ret;
	if (sizeof (long double) == sizeof (NativeLongDouble)) {
		set_softfloat_exception_flags (sfloat_16->to_long_double (bits_, reinterpret_cast <NativeLongDouble&> (ret)));
	} else {
		// MSVC
		assert (sizeof (long double) == sizeof (double));
		set_softfloat_exception_flags (sfloat_16->to_double (bits_, reinterpret_cast <NativeDouble&> (ret)));
	}
	return ret;
}

inline
SoftFloat <16>::operator int32_t () const
{
	int32_t ret;
	set_softfloat_exception_flags (sfloat_16->to_long (bits_, ret));
	return ret;
}

inline
SoftFloat <16>::operator int64_t () const
{
	int64_t ret;
	set_softfloat_exception_flags (sfloat_16->to_long_long (bits_, ret));
	return ret;
}

inline
SoftFloat <16>& SoftFloat <16>::operator += (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_16->add (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <16>& SoftFloat <16>::operator -= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_16->sub (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <16>& SoftFloat <16>::operator *= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_16->mul (bits_, rhs.bits_));
	return *this;
}

inline
SoftFloat <16>& SoftFloat <16>::operator /= (const SoftFloat& rhs)
{
	set_softfloat_exception_flags (sfloat_16->div (bits_, rhs.bits_));
	return *this;
}

inline
bool SoftFloat <16>::eq (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_16->eq (bits_, rhs.bits_, ret));
	return ret;
}

inline
bool SoftFloat <16>::le (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_16->le (bits_, rhs.bits_, ret));
	return ret;
}

inline
bool SoftFloat <16>::lt (const SoftFloat& rhs) const
{
	bool ret;
	set_softfloat_exception_flags (sfloat_16->lt (bits_, rhs.bits_, ret));
	return ret;
}

// Berkley SoftFloat exception flags
enum {
	softfloat_flag_inexact = 1,
	softfloat_flag_underflow = 2,
	softfloat_flag_overflow = 4,
	softfloat_flag_infinite = 8,
	softfloat_flag_invalid = 16
};

}
}

#endif
