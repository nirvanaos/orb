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
#ifndef NIRVANA_ORB_FLOAT_TYPES_H_
#define NIRVANA_ORB_FLOAT_TYPES_H_
#pragma once
#include <limits>
#include <type_traits>

namespace CORBA {
namespace Internal {

// TODO: Implement SoftFloat classes based on the SoftFloat library.
template <size_t size>
class SoftFloat
{
public:
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
};

}

typedef float Float;

static_assert (std::numeric_limits <long double>::is_iec559, "Unknown long double representation");
typedef double Double;

static_assert (sizeof (long double) != 16 || std::numeric_limits <double>::is_iec559, "Unknown double representation");
typedef std::conditional <sizeof (long double) == 16, long double, Internal::SoftFloat <16> >::type LongDouble;

}

#endif
