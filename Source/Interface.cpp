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
#include <CORBA/Interface.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/Environment.h>

namespace CORBA {
namespace Internal {

Interface* interface_duplicate (Interface* itf)
{
	if (itf) {
		Environment env;
		itf = (itf->_epv ().duplicate) (itf, &env);
		env.check ();
	}
	return itf;
}

void interface_release (Interface* itf) NIRVANA_NOEXCEPT
{
	if (itf) {
		try {
			(itf->_epv ().release) (itf);
		} catch (...) {}
	}
}

Interface* Interface::_check (Interface* bridge, String_in interface_id)
{
	if (bridge) {
		//  _check_pointer (bridge);
		const Char* bridge_id = bridge->_epv ().interface_id;
		if (!RepositoryId::compatible (bridge_id, interface_id))
			::Nirvana::throw_INV_OBJREF ();
	}
	return bridge;
}

}
}
	