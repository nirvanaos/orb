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
#include <CORBA/CORBA.h>

namespace CORBA {
namespace Internal {

void check_BCD (const Octet* bcd, UShort digits)
{
	assert (bcd);

	if (digits) {
		size_t len = (digits + 2) / 2;
		Octet oct = bcd [len - 1];
		Octet sign = oct & 0x0F;
		if (sign == 0xC || sign == 0xD) {
			if ((oct & 0xF0) <= 0x90) {
				for (const Octet* p = bcd, *end = p + len - 1; p != end; ++p) {
					oct = *p;
					if ((oct & 0xF0) > 0x90 || (oct & 0x0F) > 0x09)
						throw BAD_PARAM ();
				}
				return; // OK
			}
		}
	}
	throw BAD_PARAM ();
}

}
}
