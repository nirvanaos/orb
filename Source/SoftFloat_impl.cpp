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

void set_softfloat_exception_flags (unsigned softfloat_flags) noexcept
{
	int f = 0;
	if (softfloat_flags & softfloat_flag_inexact)
		f |= FE_INEXACT;
	if (softfloat_flags & softfloat_flag_underflow)
		f |= FE_UNDERFLOW;
	if (softfloat_flags & softfloat_flag_infinite)
		f |= FE_DIVBYZERO;
	if (softfloat_flags & softfloat_flag_invalid)
		f |= FE_INVALID;
	feraiseexcept (f);
}

}
}
