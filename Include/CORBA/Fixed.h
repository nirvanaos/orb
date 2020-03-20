#ifndef NIRVANA_ORB_FIXED_H_
#define NIRVANA_ORB_FIXED_H_

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
	Char *to_string () const;

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
