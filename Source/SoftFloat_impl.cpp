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

void set_softfloat_exception_flags (unsigned f)
{
	fexcept_t excepts = 0;
	if (f) {
		if (f & softfloat_flag_inexact)
			excepts |= FE_INEXACT;
		if (f & softfloat_flag_underflow)
			excepts |= FE_UNDERFLOW;
		if (f & softfloat_flag_overflow)
			excepts |= FE_OVERFLOW;
		if (f & softfloat_flag_infinite)
			excepts |= FE_DIVBYZERO;
		if (f & softfloat_flag_invalid)
			excepts |= FE_INVALID;
	}
	fesetexceptflag (&excepts, FE_ALL_EXCEPT);
}

}
}
