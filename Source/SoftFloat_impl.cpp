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
#include "../../pch/pch.h"
#include <fenv.h>

namespace CORBA {
namespace Internal {

// Berkley SoftFloat exception flags
enum {
	softfloat_flag_inexact = 1,
	softfloat_flag_underflow = 2,
	softfloat_flag_overflow = 4,
	softfloat_flag_infinite = 8,
	softfloat_flag_invalid = 16
};

static_assert (softfloat_flag_inexact == FE_INEXACT, "FE_INEXACT");
static_assert (softfloat_flag_underflow == FE_UNDERFLOW, "FE_UNDERFLOW");
static_assert (softfloat_flag_overflow == FE_OVERFLOW, "FE_OVERFLOW");
static_assert (softfloat_flag_infinite == FE_DIVBYZERO, "FE_DIVBYZERO");
static_assert (softfloat_flag_invalid == FE_INVALID, "FE_INVALID");

void set_softfloat_exception_flags (unsigned f)
{
	feraiseexcept (f);
}

}
}
