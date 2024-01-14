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
#include <CORBA/ServantBasePOA.h>

namespace CORBA {
namespace Internal {

ServantPOA <PortableServer::ServantBase>::ServantPOA () :
	ServantBaseLink (Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>::epv_)
{}

ULong ServantPOA <PortableServer::ServantBase>::_refcount_value ()
{
	return ServantBaseLink::_refcount_value ();
}

Boolean ServantPOA <PortableServer::ServantBase>::_is_a (String_in type_id)
{
	return ServantBaseLink::_is_a (type_id);
}

}
}