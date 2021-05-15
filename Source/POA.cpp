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
#include <CORBA/CORBA.h>
#include <CORBA/POA.h>
#include <Nirvana/ImportInterface.h>

NIRVANA_OLF_SECTION_N (0) extern const Nirvana::ImportInterfaceT <CORBA::TypeCode>
PortableServer::_tc_POA = { Nirvana::OLF_IMPORT_INTERFACE, PortableServer::POA::repository_id_, CORBA::TypeCode::repository_id_ };

CORBA::TypeCode::_ptr_type CORBA::Internal::Type <PortableServer::POA>::type_code ()
{
	return PortableServer::_tc_POA;
}

NIRVANA_EXCEPTION_DEF (CORBA::Internal::Definitions <PortableServer::POA>::, ServantAlreadyActive);

NIRVANA_OLF_SECTION_N (1) const Nirvana::ImportInterfaceT <CORBA::TypeCode>
CORBA::Internal::Definitions <PortableServer::POA>::_tc_ServantAlreadyActive = { Nirvana::OLF_IMPORT_INTERFACE,
CORBA::Internal::RepIdOf <CORBA::Internal::Definitions <PortableServer::POA>::ServantAlreadyActive>::repository_id_, CORBA::TypeCode::repository_id_ };

NIRVANA_EXCEPTION_DEF (CORBA::Internal::Definitions <PortableServer::POA>::, ObjectNotActive);

NIRVANA_OLF_SECTION_N (2) const Nirvana::ImportInterfaceT <CORBA::TypeCode>
CORBA::Internal::Definitions <PortableServer::POA>::_tc_ObjectNotActive = { Nirvana::OLF_IMPORT_INTERFACE,
CORBA::Internal::RepIdOf <CORBA::Internal::Definitions <PortableServer::POA>::ObjectNotActive>::repository_id_, CORBA::TypeCode::repository_id_ };

