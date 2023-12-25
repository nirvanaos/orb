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
#include <stddef.h>

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

template <> class SoftFloat <4>
{
public:
	// Constructors
	SoftFloat () {}
	SoftFloat (const SoftFloat& val) = default;

	SoftFloat (float val);
	SoftFloat (double val);
	SoftFloat (long double val);
	SoftFloat (long val);
	SoftFloat (unsigned long val);
	SoftFloat (long long val);
	SoftFloat (unsigned long long val);

	// Conversions
	operator float () const;
	operator double () const;
	operator long double () const;
	operator long () const;
	operator long long () const;

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
	bool operator! () const;

private:
	uint32_t d_;
};

template <> class SoftFloat <8>
{
public:
	// Constructors
	SoftFloat () {}
	SoftFloat (const SoftFloat& val) = default;

	SoftFloat (float val);
	SoftFloat (double val);
	SoftFloat (long double val);
	SoftFloat (long val);
	SoftFloat (unsigned long val);
	SoftFloat (long long val);
	SoftFloat (unsigned long long val);

	// Conversions
	operator float () const;
	operator double () const;
	operator long double () const;
	operator long () const;
	operator long long () const;

	// Operators
	SoftFloat& operator = (const SoftFloat& val);
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
	bool operator! () const;

private:
	uint64_t d_;
};

template <> class SoftFloat <16>
{
public:
	// Constructors
	SoftFloat () {}
	SoftFloat (const SoftFloat& val) = default;

	SoftFloat (float val);
	SoftFloat (double val);
	SoftFloat (long double val);
	SoftFloat (long val);
	SoftFloat (unsigned long val);
	SoftFloat (long long val);
	SoftFloat (unsigned long long val);

	// Conversions
	operator float () const;
	operator double () const;
	operator long double () const;
	operator long () const;
	operator long long () const;

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
	bool operator! () const;

private:
	uint64_t d_ [2];
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
