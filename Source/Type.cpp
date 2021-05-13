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
#include <CORBA/Type.h>
#include <Nirvana/throw_exception.h>
#include <exception>

namespace CORBA {
namespace Internal {

void _check_pointer (const void* p)
{
	if (!p)
		::Nirvana::throw_BAD_PARAM ();	// Invalid pointer
}

bool uncaught_exception ()
{
#ifdef NIRVANA_C17
	return std::uncaught_exceptions () != 0;
#else
	return std::uncaught_exception ();
#endif
}

}
}
