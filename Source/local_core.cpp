/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#include <CORBA/local_core.h>

namespace CORBA {
namespace Nirvana {

Bridge <Object>* get_object_from_core (LocalObject_ptr core_object, String_in iid)
{
	Bridge <LocalObject>* bridge = &core_object;
	Environment env;
	Bridge <Object>* obj = bridge->_epv ().base.CORBA_Object (bridge, iid, &env);
	env.check ();
	return obj;
}

Interface* get_proxy (LocalObject_ptr core_object)
{
	Interface* proxy = AbstractBase_ptr (Object_ptr (core_object))->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

}
}
