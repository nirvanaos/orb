/// \file
/// \brief Software floating point support
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
#include <Nirvana/bitutils.h>
#include <algorithm>

NIRVANA_STD_BEGIN
template <class> struct char_traits;
template <class, class> class basic_istream;
template <class, class> class basic_ostream;
typedef basic_istream <char, char_traits <char> > istream;
typedef basic_ostream <char, char_traits <char> > ostream;
NIRVANA_STD_END

namespace CORBA {
namespace Internal {

/// IEEE 754 floating point type binary representation
/// 
/// \tparam size Size in bytes.
template <size_t size>
struct alignas (size <= 8 ? size : 8) FloatIEEE
{
	typedef uint32_t Word;
	static const size_t WORD_CNT = size / sizeof (Word);
	Word bits [WORD_CNT];

	static const size_t SIGN_INDEX = Nirvana::endian::native == Nirvana::endian::little ? WORD_CNT - 1 : 0;
	static const Word SIGN = ~((Word)(~0) >> 1);

	void zero () noexcept
	{
		std::fill_n (bits, WORD_CNT, 0);
	}

	template <size_t i>
	Word get_bits () const noexcept
	{
		if (i == SIGN_INDEX)
			return bits [i] & ~SIGN;
		else
			return bits [i];
	}

	bool is_zero () const noexcept
	{
		Word mask = get_bits <0> ();
		if (WORD_CNT > 1) {
			mask |= get_bits <1> ();
			if (WORD_CNT > 2) {
				mask |= get_bits <2> ();
				mask |= get_bits <3> ();
				if (WORD_CNT > 4) {
					mask |= get_bits <4> ();
					mask |= get_bits <5> ();
					mask |= get_bits <6> ();
					mask |= get_bits <7> ();
				}
			}
		}
		return mask == 0;
	}

	FloatIEEE negate () const noexcept
	{
		FloatIEEE ret = *this;
		ret.bits [SIGN_INDEX] ^= SIGN;
		return ret;
	}

	bool equal (const FloatIEEE& rhs) const noexcept
	{
		if (is_zero ())
			return rhs.is_zero ();
		else
			return std::equal (bits, bits + WORD_CNT, rhs.bits);
	}
};

/// Software IEEE 754 floating point type implementation class template.
/// 
/// \tparam size Size in bytes.
template <size_t size> class SoftFloat;

template <> class SoftFloat <4>
{
public:
	typedef float NativeType;

	// Constructors
	SoftFloat ()
	{
		bits_.zero ();
	}

	SoftFloat (const SoftFloat& val) noexcept = default;

	inline SoftFloat (const NativeType& val);
	inline SoftFloat (const int32_t& val);
	inline SoftFloat (const uint32_t& val);
	inline SoftFloat (const int64_t& val);
	inline SoftFloat (const uint64_t& val);

	// Conversions
	inline operator NativeType () const;
	inline operator int32_t () const;
	inline operator int64_t () const;

	// Operators
	inline SoftFloat& operator = (const SoftFloat& val) noexcept = default;
	inline SoftFloat& operator += (const SoftFloat& val);
	inline SoftFloat& operator -= (const SoftFloat& val);
	inline SoftFloat& operator *= (const SoftFloat& val);
	inline SoftFloat& operator /= (const SoftFloat& val);
	inline SoftFloat& operator ++ ();
	inline SoftFloat operator ++ (int);
	inline SoftFloat& operator -- ();
	inline SoftFloat operator -- (int);

	inline SoftFloat operator + () const noexcept
	{
		return *this;
	}

	inline SoftFloat operator - () const noexcept
	{
		return SoftFloat (bits_.negate ());
	}

	bool operator! () const noexcept
	{
		return bits_.is_zero ();
	}

	// Comparation

	inline int compare (const SoftFloat& rhs) const;

	bool equal (const SoftFloat& rhs) const noexcept
	{
		return bits_.equal (rhs.bits_);
	}

private:
	SoftFloat (const FloatIEEE <4>& bits) noexcept :
		bits_ (bits)
	{}

private:
	FloatIEEE <4> bits_;
};

template <> class SoftFloat <8>
{
public:
	typedef double NativeType;

	// Constructors

	SoftFloat ()
	{
		bits_.zero ();
	}

	SoftFloat (const SoftFloat& val) noexcept = default;

	inline SoftFloat (const NativeType& val);
	inline SoftFloat (const int32_t& val);
	inline SoftFloat (const uint32_t& val);
	inline SoftFloat (const int64_t& val);
	inline SoftFloat (const uint64_t& val);

	// Conversions
	inline operator NativeType () const;
	inline operator int32_t () const;
	inline operator int64_t () const;

	// Operators
	SoftFloat& operator = (const SoftFloat& val) noexcept = default;

	inline SoftFloat& operator += (const SoftFloat& rhs);
	inline SoftFloat& operator -= (const SoftFloat& rhs);
	inline SoftFloat& operator *= (const SoftFloat& rhs);
	inline SoftFloat& operator /= (const SoftFloat& rhs);

	inline SoftFloat& operator ++ ()
	{
		return operator += (SoftFloat ((uint32_t)1));
	}

	inline SoftFloat operator ++ (int)
	{
		SoftFloat tmp (*this);
		operator += (SoftFloat ((uint32_t)1));
		return tmp;
	}

	inline SoftFloat& operator -- ()
	{
		return operator -= (SoftFloat ((uint32_t)1));
	}

	inline SoftFloat operator -- (int)
	{
		SoftFloat tmp (*this);
		operator -= (SoftFloat ((uint32_t)1));
		return tmp;
	}

	inline const SoftFloat& operator + () const noexcept
	{
		return *this;
	}

	inline SoftFloat operator - () const noexcept
	{
		return SoftFloat (bits_.negate ());
	}

	bool operator! () const noexcept
	{
		return bits_.is_zero ();
	}

	// Comparation
	inline int compare (const SoftFloat& rhs) const;

	bool equal (const SoftFloat& rhs) const noexcept
	{
		return bits_.equal (rhs.bits_);
	}

private:
	SoftFloat (const FloatIEEE <8>& bits) noexcept :
		bits_ (bits)
	{}

private:
	FloatIEEE <8> bits_;
};

template <> class SoftFloat <16>
{
public:
	typedef long double NativeType;

	// Constructors

	SoftFloat ()
	{
		bits_.zero ();
	}

	SoftFloat (const SoftFloat& val) noexcept = default;
	inline SoftFloat (const NativeType& val);
	inline SoftFloat (const int32_t& val);
	inline SoftFloat (const uint32_t& val);
	inline SoftFloat (const int64_t& val);
	inline SoftFloat (const uint64_t& val);

	// Conversions
	inline operator NativeType () const;
	inline operator int32_t () const;
	inline operator int64_t () const;

	// Operators
	inline SoftFloat& operator = (const SoftFloat& val) = default;
	inline SoftFloat& operator += (const SoftFloat& rhs);
	inline SoftFloat& operator -= (const SoftFloat& rhs);
	inline SoftFloat& operator *= (const SoftFloat& rhs);
	inline SoftFloat& operator /= (const SoftFloat& rhs);
	inline SoftFloat& operator ++ ();
	inline SoftFloat operator ++ (int);
	inline SoftFloat& operator -- ();
	inline SoftFloat operator -- (int);
	inline SoftFloat operator + () const;
	inline SoftFloat operator - () const;

	bool operator! () const noexcept
	{
		return bits_.is_zero ();
	}

	// Comparation
	inline int compare (const SoftFloat& rhs) const;

	bool equal (const SoftFloat& rhs) const noexcept
	{
		return bits_.equal (rhs.bits_);
	}

private:
	SoftFloat (const FloatIEEE <16>& bits) noexcept :
		bits_ (bits)
	{}

private:
	FloatIEEE <16> bits_;
};

template <size_t size> inline
std::istream& operator >> (std::istream& is, SoftFloat <size>& val)
{
	typename SoftFloat <size>::NativeType nt;
	is >> nt;
	val = SoftFloat <size> (nt);
	return is;
}

template <size_t size> inline
std::ostream& operator << (std::ostream& os, const SoftFloat <size>& val)
{
	return os << static_cast <typename SoftFloat <size>::NativeType> (val);
}

template <size_t size> inline
SoftFloat <size> operator + (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	SoftFloat <size> res (val1);
	res += val2;
	return res;
}

template <size_t size> inline
SoftFloat <size> operator - (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	SoftFloat <size> res (val1);
	res -= val2;
	return res;
}

template <size_t size> inline
SoftFloat <size> operator * (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	SoftFloat <size> res (val1);
	res *= val2;
	return res;
}

template <size_t size> inline
SoftFloat <size> operator / (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	SoftFloat <size> res (val1);
	res /= val2;
	return res;
}

template <size_t size> inline
bool operator > (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return val1.compare (val2) > 0;
}

template <size_t size> inline
bool operator < (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return val1.compare (val2) < 0;
}

template <size_t size> inline
bool operator >= (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return val1.compare (val2) >= 0;
}

template <size_t size> inline
bool operator <= (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return val1.compare (val2) <= 0;
}

template <size_t size> inline
bool operator == (const SoftFloat <size>& val1, const SoftFloat <size>& val2) noexcept
{
	return val1.equal (val2);
}

template <size_t size> inline
bool operator != (const SoftFloat <size>& val1, const SoftFloat <size>& val2) noexcept
{
	return !val1.equal (val2);
}

}
}
#endif
