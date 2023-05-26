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
#include <CORBA/ServantBaseStatic.h>
#include <CORBA/LocalObjectStatic.h>

namespace CORBA {
namespace Internal {

void ServantBaseStaticDummy::__add_ref (Bridge <PortableServer::ServantBase>* obj, Interface* env)
{
}

void ServantBaseStaticDummy::__remove_ref (Bridge <PortableServer::ServantBase>* obj, Interface* env)
{
}

ULong ServantBaseStaticDummy::__refcount_value (Bridge <PortableServer::ServantBase>* obj, Interface* env)
{
	return 1;
}

void ServantBaseStaticDummy::__delete_object (Bridge <PortableServer::ServantBase>* _b, Interface* _env)
{
	set_NO_IMPLEMENT (_env);
}

void LocalObjectStaticDummy::__add_ref (Bridge <LocalObject>* obj, Interface* env)
{
}

void LocalObjectStaticDummy::__remove_ref (Bridge <LocalObject>* obj, Interface* env)
{
}

ULong LocalObjectStaticDummy::__refcount_value (Bridge <LocalObject>* obj, Interface* env)
{
	return 1;
}

void LocalObjectStaticDummy::__delete_object (Bridge <LocalObject>* _b, Interface* _env)
{
	set_NO_IMPLEMENT (_env);
}

}
}
