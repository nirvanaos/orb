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
#include <CORBA/FindInterface.h>

namespace CORBA {
namespace Internal {

Interface* InterfaceEntry::find (const InterfaceEntry* begin, const InterfaceEntry* end,
	void* servant, String_in id) noexcept
{
	const InterfaceEntry* ie = 0;
	if (id.empty ()) // On NULL id return primary interface
		ie = begin;
	else {
		for (const InterfaceEntry* p = begin; p != end; ++p) {
			if (RepId::compatible (p->interface_id, p->id_len, id)) {
				ie = p;
				break;
			}
		}
	}
	if (ie) {
		Interface* itf = (ie->cast) (servant);
		assert (!strcmp (itf->_epv ().interface_id, ie->interface_id));
		return itf;
	} else
		return nullptr;
}

}
}
