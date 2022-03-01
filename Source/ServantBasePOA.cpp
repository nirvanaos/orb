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

ServantPOA <PortableServer::ServantBase>::ServantPOA () :
	ServantBaseLink (Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>::epv_)
{}

I_ref <InterfaceDef> ServantPOA <PortableServer::ServantBase>::_get_interface ()
{
	_check_construct ();
	return ServantBaseLink::_get_interface ();
}

I_ref <Interface> ServantPOA <PortableServer::ServantBase>::_get_proxy ()
{
	_check_construct ();
	return ServantBaseLink::_get_proxy ();
}

}
}