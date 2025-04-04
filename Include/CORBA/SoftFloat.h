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
	typedef unsigned int Word;
	static const size_t WORD_CNT = size / sizeof (Word);
	Word bits [WORD_CNT];

	static const size_t SIGN_INDEX = Nirvana::endian::native == Nirvana::endian::little ? WORD_CNT - 1 : 0;
	static const Word SIGN = ~((Word)(~0) >> 1);

	void zero () noexcept
	{
		std::fill_n (bits, WORD_CNT, 0);
	}

	// Compiler should unroll the recursion
	NIRVANA_CONSTEXPR20 Word get_bits (size_t i) const noexcept
	{
		Word ret = bits [i];
		if (i == SIGN_INDEX)
			ret &= ~SIGN;
		if (i > 0)
			ret |= get_bits (i - 1);
		return ret;
	}

	bool is_zero () const noexcept
	{
		return get_bits (WORD_CNT - 1) == 0;
	}

	FloatIEEE negate () const noexcept
	{
		FloatIEEE ret = *this;
		ret.bits [SIGN_INDEX] ^= SIGN;
		return ret;
	}

	FloatIEEE copy_sign (const FloatIEEE& sign) const noexcept
	{
		FloatIEEE ret = *this;
		ret.bits [SIGN_INDEX] = (ret.bits [SIGN_INDEX] & (~SIGN)) | (sign.bits [SIGN_INDEX] & SIGN);
		return ret;
	}

	Word sign_bit () const noexcept
	{
		return bits [SIGN_INDEX] & SIGN;
	}

	FloatIEEE abs () const noexcept
	{
		FloatIEEE ret = *this;
		if (bits [SIGN_INDEX] & SIGN)
			ret.bits [SIGN_INDEX] &= ~SIGN;
		return ret;
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

	inline SoftFloat (const SoftFloat& val) noexcept = default;
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

	inline bool eq (const SoftFloat& rhs) const;
	inline bool le (const SoftFloat& rhs) const;
	inline bool lt (const SoftFloat& rhs) const;

	// Sign operations

	SoftFloat copy_sign (const SoftFloat& sign) const noexcept
	{
		return SoftFloat (bits_.copy_sign (sign.bits_));
	}

	bool sign_bit () const noexcept
	{
		return bits_.sign_bit () != 0;
	}

	SoftFloat abs () const noexcept
	{
		return SoftFloat (bits_.abs ());
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

	inline SoftFloat (const SoftFloat& val) noexcept = default;
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

	inline bool eq (const SoftFloat& rhs) const;
	inline bool le (const SoftFloat& rhs) const;
	inline bool lt (const SoftFloat& rhs) const;

	// Sign operations

	SoftFloat copy_sign (const SoftFloat& sign) const noexcept
	{
		return SoftFloat (bits_.copy_sign (sign.bits_));
	}

	bool sign_bit () const noexcept
	{
		return bits_.sign_bit () != 0;
	}

	SoftFloat abs () const noexcept
	{
		return SoftFloat (bits_.abs ());
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

	inline SoftFloat (const SoftFloat& val) noexcept = default;
	inline SoftFloat (const double& val);
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

	inline bool eq (const SoftFloat& rhs) const;
	inline bool le (const SoftFloat& rhs) const;
	inline bool lt (const SoftFloat& rhs) const;

	// Sign operations

	SoftFloat copy_sign (const SoftFloat& sign) const noexcept
	{
		return SoftFloat (bits_.copy_sign (sign.bits_));
	}

	bool sign_bit () const noexcept
	{
		return bits_.sign_bit () != 0;
	}

	SoftFloat abs () const noexcept
	{
		return SoftFloat (bits_.abs ());
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
	typename SoftFloat <size>::NativeType nt = 0;
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
SoftFloat <size> operator + (const SoftFloat <size>& val1, const typename SoftFloat <size>::NativeType& val2)
{
	SoftFloat <size> res (val1);
	res += val2;
	return res;
}

template <size_t size> inline
SoftFloat <size> operator + (const typename SoftFloat <size>::NativeType& val1, const SoftFloat <size>& val2)
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
	return !val1.le (val2);
}

template <size_t size> inline
bool operator < (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return val1.lt (val2);
}

template <size_t size> inline
bool operator >= (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return !val1.lt (val2);
}

template <size_t size> inline
bool operator <= (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return val1.le (val2);
}

template <size_t size> inline
bool operator == (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return val1.eq (val2);
}

template <size_t size> inline
bool operator == (const SoftFloat <size>& val1, const typename SoftFloat <size>::NativeType& val2)
{
	return val1.eq (SoftFloat <size> (val2));
}

template <size_t size> inline
bool operator == (const typename SoftFloat <size>::NativeType& val1, const SoftFloat <size>& val2)
{
	return SoftFloat <size> (val1).eq (val2);
}

template <size_t size> inline
bool operator != (const SoftFloat <size>& val1, const SoftFloat <size>& val2)
{
	return !operator == (val1, val2);
}

template <size_t size> inline
bool operator != (const SoftFloat <size>& val1, const typename SoftFloat <size>::NativeType& val2)
{
	return !operator == (val1, val2);
}

template <size_t size> inline
bool operator != (const typename SoftFloat <size>::NativeType& val1, const SoftFloat <size>& val2)
{
	return !operator == (val1, val2);
}

}
}

namespace std {

template <size_t size> inline
CORBA::Internal::SoftFloat <size> copysign (const CORBA::Internal::SoftFloat <size>& x, const CORBA::Internal::SoftFloat <size>& y)
{
	return x.copy_sign (y);
}

template <size_t size> inline
CORBA::Internal::SoftFloat <size> abs (const CORBA::Internal::SoftFloat <size>& arg)
{
	return arg.abs ();
}

template <size_t size> inline
bool signbit (const CORBA::Internal::SoftFloat <size>& arg)
{
	return arg.sign_bit ();
}

}

#endif
