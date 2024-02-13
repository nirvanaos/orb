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

namespace CORBA {
namespace Internal {

void LocalObjectLink::_create_proxy (Object::_ptr_type comp)
{
	g_object_factory->create_local_object (this, &core_object_, comp);
}

Object::_ptr_type LocalObjectLink::_object ()
{
	if (!core_object_)
		_create_proxy (nullptr);

	Environment env;
	Object::_ptr_type ret (static_cast <Object*> (_get_object (&Type <IDL::String>::C_in (RepIdOf <Object>::id), &env)));
	env.check ();
	if (!ret)
		throw MARSHAL ();
	return ret;
}

}
}
