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

#include "SoftFloat.h"
#include <limits>
#include <type_traits>

namespace CORBA {

typedef std::conditional <sizeof (float) == 4 && std::numeric_limits <float>::is_iec559, float, Internal::SoftFloat <4> >::type Float;
typedef std::conditional <sizeof (double) == 8 && std::numeric_limits <double>::is_iec559, double, Internal::SoftFloat <8> >::type Double;
typedef std::conditional <sizeof (long double) == 16 && std::numeric_limits <long double>::is_iec559, long double, Internal::SoftFloat <16> >::type LongDouble;

}

#endif
