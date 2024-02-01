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
#ifndef NIRVANA_ORB_SOFTFLOAT_H_
#define NIRVANA_ORB_SOFTFLOAT_H_
#pragma once

#include <Nirvana/NirvanaBase.h>

NIRVANA_STD_BEGIN
template <class> struct char_traits;
template <class, class> class basic_istream;
template <class, class> class basic_ostream;
typedef basic_istream <char, char_traits <char> > istream;
typedef basic_ostream <char, char_traits <char> > ostream;
NIRVANA_STD_END

namespace CORBA {
namespace Internal {

///  Software floating point support.
template <size_t size> class SoftFloat;

typedef uint32_t FloatIEEE4;
typedef uint64_t FloatIEEE8;

struct FloatIEEE16
{
	uint64_t bits [2];
};

template <> class SoftFloat <4>
{
public:
	// Constructors
	SoftFloat () :
		bits_ (0)
	{}

	SoftFloat (const SoftFloat& val) noexcept = default;

	SoftFloat (float val);
	SoftFloat (const int32_t& val);
	SoftFloat (const uint32_t& val);
	SoftFloat (const int64_t& val);
	SoftFloat (const uint64_t& val);

	// Conversions
	operator float () const;
	operator int32_t () const;
	operator int64_t () const;

	// Operators
	SoftFloat& operator = (const SoftFloat& val) noexcept = default;
	SoftFloat& operator += (const SoftFloat& val);
	SoftFloat& operator -= (const SoftFloat& val);
	SoftFloat& operator *= (const SoftFloat& val);
	SoftFloat& operator /= (const SoftFloat& val);
	SoftFloat& operator ++ ();
	SoftFloat operator ++ (int);
	SoftFloat& operator -- ();
	SoftFloat operator -- (int);
	SoftFloat operator + () const;
	SoftFloat operator - () const;

	bool operator! () const noexcept
	{
		return !bits_;
	}

private:
	FloatIEEE4 bits_;
};

template <> class SoftFloat <8>
{
public:
	// Constructors
	SoftFloat () :
		bits_ (0)
	{}

	SoftFloat (const SoftFloat& val) noexcept = default;

	SoftFloat (float val);
	SoftFloat (double val);
	SoftFloat (const int32_t& val);
	SoftFloat (const uint32_t& val);
	SoftFloat (const int64_t& val);
	SoftFloat (const uint64_t& val);

	// Conversions
	operator float () const;
	operator double () const;
	operator int32_t () const;
	operator int64_t () const;

	// Operators
	SoftFloat& operator = (const SoftFloat& val) noexcept = default;

	SoftFloat& operator += (const SoftFloat& val);
	SoftFloat& operator -= (const SoftFloat& val);
	SoftFloat& operator *= (const SoftFloat& val);
	SoftFloat& operator /= (const SoftFloat& val);
	SoftFloat& operator ++ ();
	SoftFloat operator ++ (int);
	SoftFloat& operator -- ();
	SoftFloat operator -- (int);
	SoftFloat operator + () const;
	SoftFloat operator - () const;

	bool operator! () const noexcept
	{
		return !bits_;
	}

private:
	FloatIEEE8 bits_;
};

template <> class SoftFloat <16>
{
public:
	// Constructors
	SoftFloat () :
		bits_{ 0, 0 }
	{}

	SoftFloat (const SoftFloat& val) noexcept = default;
	SoftFloat (const float& val);
	SoftFloat (const double& val);
	SoftFloat (const long double& val);
	SoftFloat (const int32_t& val);
	SoftFloat (const uint32_t& val);
	SoftFloat (const int64_t& val);
	SoftFloat (const uint64_t& val);

	// Conversions
	operator float () const;
	operator double () const;
	operator long double () const;
	operator int32_t () const;
	operator int64_t () const;

	// Operators
	SoftFloat& operator = (const SoftFloat& val) = default;
	SoftFloat& operator += (const SoftFloat& val);
	SoftFloat& operator -= (const SoftFloat& val);
	SoftFloat& operator *= (const SoftFloat& val);
	SoftFloat& operator /= (const SoftFloat& val);
	SoftFloat& operator ++ ();
	SoftFloat operator ++ (int);
	SoftFloat& operator -- ();
	SoftFloat operator -- (int);
	SoftFloat operator + () const;
	SoftFloat operator - () const;

	bool operator! () const noexcept
	{
		return 0 == bits_.bits [0] && 0 == bits_.bits [1];
	}

private:
	FloatIEEE16 bits_;
};

template <size_t size>
std::istream& operator >> (std::istream& is, SoftFloat <size>& val);

template <size_t size>
std::ostream& operator << (std::ostream& os, const SoftFloat <size>& val);

template <size_t size>
SoftFloat <size> operator + (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
SoftFloat <size> operator - (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
SoftFloat <size> operator * (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
SoftFloat <size> operator / (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
bool operator > (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
bool operator < (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
bool operator >= (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
bool operator <= (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
bool operator == (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

template <size_t size>
bool operator != (const SoftFloat <size>& val1, const SoftFloat <size>& val2);

}
}
#endif
