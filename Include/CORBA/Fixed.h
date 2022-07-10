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
#ifndef NIRVANA_ORB_FIXED_H_
#define NIRVANA_ORB_FIXED_H_
#pragma once

#include "Type_array.h"
#include <Nirvana/basic_string.h>
#include <Nirvana/DecCalc.h>
#include <iosfwd>

namespace Nirvana {

extern const ImportInterfaceT <DecCalc> g_dec_calc;

}

namespace IDL {

/// Fixed point common data representation
template <uint16_t digits, uint16_t scale>
struct FixedCDR
{
	static_assert (digits <= 31, "digits <= 31");

	uint8_t bcd [(digits + 2) / 2];
};

}

namespace CORBA {

/// Fixed point value
class Fixed
{
public:
	///@{
	/// Constructors

	Fixed (const Fixed& val) = default;

	Fixed (Long val = 0)
	{
		Nirvana::g_dec_calc->from_long (val_, val);
	}

	Fixed (ULong val)
	{
		Nirvana::g_dec_calc->from_ulong (val_, val);
	}

	Fixed (LongLong val)
	{
		Nirvana::g_dec_calc->from_longlong (val_, val);
	}

	Fixed (ULongLong val)
	{
		Nirvana::g_dec_calc->from_ulonglong (val_, val);
	}

	///@{
	/// CORBA::Double and CORBA::LongDouble may be emulated so we use native floating point types.

	Fixed (double val);
	Fixed (long double val);

	///@}

	explicit Fixed (const std::string& s)
	{
		Nirvana::g_dec_calc->from_string (val_, s.c_str ());
	}

	Fixed (const char* s)
	{
		Nirvana::g_dec_calc->from_string (val_, s);
	}

	template <UShort digits, UShort scale>
	Fixed (const IDL::FixedCDR <digits, scale>& cdr)
	{
		Nirvana::g_dec_calc->from_BCD (val_, digits, scale, cdr.bcd);
	}

	///@}

	///@{
	/// Conversions

	operator LongLong () const
	{
		return Nirvana::g_dec_calc->to_longlong (val_);
	}

	operator long double () const;

	operator const Nirvana::DecCalc::Number () const
	{
		return val_;
	}

	/// Converts a fixed value to a string
	/// 
	/// Leading zeros are dropped, but trailing fractional zeros are preserved.
	/// (For example, a fixed<4, 2> with the value 1.1 is converted “1.10”).
	/// 
	/// \returns string
	std::string to_string () const
	{
		return Nirvana::g_dec_calc->to_string (val_);
	}

	///@}

	///@{
	/** Rounding

	The round and truncate functions convert a fixed value to a new value with the specified scale. If the new scale
	requires the value to lose precision on the right, the round function will round away from zero values that are halfway or
	more to the next absolute value for the new fixed precision.The truncate function always truncates the value towards
	zero.If the value currently has fewer digits on the right than the new scale, roundand truncate return the argument
	unmodified.For example :
	~~~~~~~~~~~~~~~~~~~~~~~
	// C++
	F f1 (“0.1”);
	F f2 (“.05”);
	F f3 (“ - 0.005”);
	~~~~~~~~~~~~~~~~~~~~~~~
	In this example, `f1.round (0)` and `f1.truncate (0)` both return 0, `f2.round (1)` returns 0.1, `f2.truncate (1)`
	returns 0.0, `f3.round (2)` returns - 0.01 and `f3.truncate (2)` returns 0.00.
	*/

	Fixed round (UShort scale) const
	{
		Fixed ret = *this;
		Nirvana::g_dec_calc->round (ret.val_, scale);
		return ret;
	}

	Fixed truncate (UShort scale) const
	{
		Fixed ret = *this;
		Nirvana::g_dec_calc->truncate (ret.val_, scale);
		return ret;
	}

	///@}

	///@{
	/// Operators

	Fixed& operator = (const Fixed& val) = default;

	Fixed& operator += (const Fixed& val)
	{
		Nirvana::g_dec_calc->add (val_, val.val_);
		return *this;
	}

	Fixed& operator -= (const Fixed& val)
	{
		Nirvana::g_dec_calc->subtract (val_, val.val_);
		return *this;
	}

	Fixed& operator *= (const Fixed& val)
	{
		Nirvana::g_dec_calc->multiply (val_, val.val_);
		return *this;
	}

	Fixed& operator /= (const Fixed& val)
	{
		Nirvana::g_dec_calc->divide (val_, val.val_);
		return *this;
	}

	Fixed& operator ++ ()
	{
		return operator += (Fixed (1));
	}

	Fixed operator ++ (int)
	{
		Fixed f (*this);
		operator ++ ();
		return f;
	}

	Fixed& operator -- ()
	{
		return operator -= (Fixed (1));
	}

	Fixed operator -- (int)
	{
		Fixed f (*this);
		operator -- ();
		return f;
	}

	Fixed operator + () const
	{
		return *this;
	}

	Fixed operator - () const
	{
		Fixed ret = *this;
		Nirvana::g_dec_calc->minus (ret.val_);
		return ret;
	}

	bool operator! () const
	{
		return Nirvana::g_dec_calc->is_zero (val_);
	}

	explicit operator bool () const
	{
		return !Nirvana::g_dec_calc->is_zero (val_);
	}

	///@}

	///@{
	/// The fixed_digits and fixed_scale functions
	/// return the smallest digits and scale value that can hold the complete fixed - point value.

	UShort fixed_digits () const
	{
		return val_.digits ();
	}

	UShort fixed_scale () const
	{
		return -val_.exponent ();
	}

	///@}

private:
	friend std::istream& operator >> (std::istream& is, Fixed& val);

	Nirvana::DecCalc::Number val_;
};

///@{
/**
The stream insertion and extraction operators << and >> convert a fixed - point value to / from a stream. These operators
insertand extract fixed-point values into the stream using the same format as for C++ floating point types. In particular, the
trailing ‘d’ or ‘D’ from the IDL fixed-point literal representation is not inserted or extracted from the stream. These
operators use all format controls appropriate to floating point defined by the stream classes except that they never use the
scientific format.
*/

std::istream& operator >> (std::istream& is, Fixed& val);
std::ostream& operator << (std::ostream& os, const Fixed& val);

///@}

///@{
/// Binary operators

inline
Fixed operator + (const Fixed& val1, const Fixed& val2)
{
	Fixed ret (val1);
	ret += val2;
	return ret;
}

inline
Fixed operator - (const Fixed& val1, const Fixed& val2)
{
	Fixed ret (val1);
	ret -= val2;
	return ret;
}

inline
Fixed operator * (const Fixed& val1, const Fixed& val2)
{
	Fixed ret (val1);
	ret *= val2;
	return ret;
}

inline
Fixed operator / (const Fixed& val1, const Fixed& val2)
{
	Fixed ret (val1);
	ret /= val2;
	return ret;
}

inline
Boolean operator > (const Fixed& val1, const Fixed& val2)
{
	return Nirvana::g_dec_calc->compare (val1, val2) > 0;
}

inline
Boolean operator < (const Fixed& val1, const Fixed& val2)
{
	return Nirvana::g_dec_calc->compare (val1, val2) < 0;
}

inline
Boolean operator >= (const Fixed& val1, const Fixed& val2)
{
	return Nirvana::g_dec_calc->compare (val1, val2) >= 0;
}

inline
Boolean operator <= (const Fixed& val1, const Fixed& val2)
{
	return Nirvana::g_dec_calc->compare (val1, val2) <= 0;
}

inline
Boolean operator == (const Fixed& val1, const Fixed& val2)
{
	return Nirvana::g_dec_calc->compare (val1, val2) == 0;
}

inline
Boolean operator != (const Fixed& val1, const Fixed& val2)
{
	return Nirvana::g_dec_calc->compare (val1, val2) != 0;
}

///@}

}

#endif
