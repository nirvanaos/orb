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

I_ref <PortableServer::POA> ServantPOA <LocalObject>::_default_POA ()
{
	return PortableServer::POA::_nil ();
}

I_ref <InterfaceDef> ServantPOA <LocalObject>::_get_interface ()
{
	_check_construct ();
	return LocalObjectLink::core_object_->_get_interface ();
}

Boolean ServantPOA <LocalObject>::_is_a (String_in type_id)
{
	_check_construct ();
	return LocalObjectLink::core_object_->_is_a (type_id);
}

Boolean ServantPOA <LocalObject>::_non_existent ()
{
	return LocalObjectLink::_non_existent ();
}

void ServantPOA <LocalObject>::_add_ref ()
{
	LocalObjectLink::_add_ref ();
}

void ServantPOA <LocalObject>::_remove_ref ()
{
	LocalObjectLink::_remove_ref ();
}

ULong ServantPOA <LocalObject>::_refcount_value ()
{
	return LocalObjectLink::_refcount_value ();
}

I_ref <Interface> ServantPOA <LocalObject>::_get_proxy ()
{
	_check_construct ();
	return LocalObjectLink::_get_proxy ();
}

Bridge <Object>* ServantPOA <LocalObject>::_get_object (String_in iid)
{
	_check_construct ();
	return LocalObjectLink::_get_object (iid);
}

}
}
