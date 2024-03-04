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
#include <CORBA/Server.h>

using namespace PortableServer;

namespace CORBA {
namespace Internal {

void ServantBaseLink::_create_proxy (Object::_ptr_type comp)
{
	object_factory->create_servant (this, &core_object_, comp);
}

Object::_ptr_type ServantBaseLink::_object ()
{
	if (!core_object_)
		_create_proxy (nullptr);

	Interface::_ptr_type proxy = core_object_->_query_interface (RepIdOf <Object>::id);
	if (!proxy)
		throw MARSHAL ();
	return proxy.template downcast <Object> ();
}

}
}
