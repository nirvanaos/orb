/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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

#include <Nirvana/basic_string.h>
#include "basic_types.h"
#include <iosfwd>

namespace CORBA {

class Fixed
{
public:
	// Constructors
	Fixed (Long val = 0);
	Fixed (ULong val);
	Fixed (LongLong val);
	Fixed (ULongLong val);
	Fixed (Double val);
	Fixed (LongDouble val);
	Fixed (const Fixed& val);
	Fixed (const char*);
	~Fixed ();

	// Conversions
	operator LongLong () const;
	operator LongDouble () const;
	Fixed round (UShort scale) const;
	Fixed truncate (UShort scale) const;
	std::string to_string () const;

	// Operators
	Fixed& operator = (const Fixed& val);
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
	UShort fixed_digits () const;
	UShort fixed_scale () const;
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
