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

	uint8_t bcd [digits + 2 / 2];
};

}

namespace CORBA {

/// Fixed point value
class Fixed
{
public:
	// Constructors
	
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

	// CORBA::Double and CORBA::LongDouble may be emulated, use native types.

	Fixed (double val);
	Fixed (long double val);

	Fixed (const Fixed& val) = default;

	explicit Fixed (const std::string& s)
	{
		Nirvana::g_dec_calc->from_string (val_, s.c_str ());
	}

	Fixed (const char* s) // For compatibility
	{
		Nirvana::g_dec_calc->from_string (val_, s);
	}

	template <UShort digits, UShort scale>
	Fixed (const IDL::FixedCDR <digits, scale>& cdr)
	{
		Nirvana::g_dec_calc->from_BCD (val_, digits, scale, cdr.bcd);
	}

	~Fixed ()
	{}

	// Conversions
	operator LongLong () const
	{
		return Nirvana::g_dec_calc->to_longlong (val_);
	}

	operator long double () const;

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
	
	std::string to_string () const
	{
		return Nirvana::g_dec_calc->to_string (val_);
	}

	// Operators
	Fixed& operator = (const Fixed& val) = default;
	Fixed& operator += (const Fixed& val);
	Fixed& operator -= (const Fixed& val);
	Fixed& operator *= (const Fixed& val);
	Fixed& operator /= (const Fixed& val);
	Fixed& operator ++ ();
	Fixed operator ++ (int);
	Fixed& operator -- ();
	Fixed operator -- (int);
	Fixed operator + () const;
	Fixed operator - () const;
	Boolean operator! () const;

	// Other member functions
	UShort fixed_digits () const
	{
		return val_.digits ();
	}

	UShort fixed_scale () const
	{
		return -val_.exponent ();
	}

	operator const Nirvana::DecCalc::Number () const
	{
		return val_;
	}

private:
	Nirvana::DecCalc::Number val_;
};

std::istream& operator >> (std::istream& is, Fixed& val);
std::ostream& operator << (std::ostream& os, const Fixed& val);
Fixed operator + (const Fixed& val1, const Fixed& val2);
Fixed operator - (const Fixed& val1, const Fixed& val2);
Fixed operator * (const Fixed& val1, const Fixed& val2);
Fixed operator / (const Fixed& val1, const Fixed& val2);
Boolean operator > (const Fixed& val1, const Fixed& val2);
Boolean operator < (const Fixed& val1, const Fixed& val2);
Boolean operator >= (const Fixed& val1, const Fixed& val2);
Boolean operator <= (const Fixed& val1, const Fixed& val2);
Boolean operator == (const Fixed& val1, const Fixed& val2);
Boolean operator != (const Fixed& val1, const Fixed& val2);

}

#endif
