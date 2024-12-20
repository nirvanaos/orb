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

namespace CORBA {
namespace Internal {

Interface::_ptr_type get_proxy (PortableServer::Servant core_object)
{
	Interface::_ptr_type proxy = core_object->_query_interface (nullptr);
	if (!proxy)
		throw MARSHAL ();
	return proxy;
}

Bridge <Object>* get_object_from_core (PortableServer::Servant core_object,
	Type <String>::ABI_in iid, Interface* env) noexcept
{
	return static_cast <Bridge <Object>*> (
		core_object->_epv ().epv.query_interface (
			static_cast <Bridge <PortableServer::ServantBase>*> (&core_object), iid, env));
}

}
}
