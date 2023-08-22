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
#include <CORBA/Server.h>

namespace CORBA {
namespace Internal {

void check_pointer (const void* p)
{
	if (!p)
		throw BAD_PARAM ();	// Invalid pointer
}

void check_pointer (const Interface* obj, const Interface::EPV& epv)
{
	check_pointer (obj);
	if (&obj->_epv () != &epv)
		throw INV_OBJREF ();	// Invalid object pointer
}

void check_pointer_noexcept (const Interface* obj, const Interface::EPV& epv,
	Interface* env) noexcept
{
	if (!obj)
		set_BAD_PARAM (env);	// Invalid pointer
	else if (&obj->_epv () != &epv)
		set_INV_OBJREF (env);
}

}
}
