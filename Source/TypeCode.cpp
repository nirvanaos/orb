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
#include <Nirvana/OLF.h>

NIRVANA_OLF_SECTION __declspec(selectany)
const Nirvana::ImportInterfaceT <CORBA::TypeCode>
CORBA::_tc_TypeCode = { Nirvana::OLF_IMPORT_INTERFACE, CORBA::TypeCode::repository_id_, CORBA::TypeCode::repository_id_ };

CORBA::TypeCode::_ptr_type CORBA::Internal::Type <CORBA::TypeCode>::type_code ()
{
	return CORBA::_tc_TypeCode;
}

NIRVANA_OLF_SECTION __declspec(selectany)
const Nirvana::ImportInterfaceT <CORBA::TypeCode>
CORBA::_tc_TCKind = { Nirvana::OLF_IMPORT_INTERFACE, CORBA::Internal::RepIdOf <CORBA::TCKind>::repository_id_, CORBA::TypeCode::repository_id_ };

NIRVANA_EXCEPTION_DEF (CORBA::Internal::Definitions <CORBA::TypeCode>::, BadKind);

NIRVANA_OLF_SECTION __declspec(selectany)
const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode>
CORBA::Internal::Definitions <CORBA::TypeCode>::_tc_BadKind = { ::Nirvana::OLF_IMPORT_INTERFACE,
CORBA::Internal::RepIdOf <CORBA::Internal::Definitions <CORBA::TypeCode>::BadKind>::repository_id_, CORBA::TypeCode::repository_id_ };

NIRVANA_EXCEPTION_DEF (CORBA::Internal::Definitions <CORBA::TypeCode>::, Bounds);

NIRVANA_OLF_SECTION __declspec(selectany)
const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode>
CORBA::Internal::Definitions <CORBA::TypeCode>::_tc_Bounds = { ::Nirvana::OLF_IMPORT_INTERFACE,
CORBA::Internal::RepIdOf <CORBA::Internal::Definitions <CORBA::TypeCode>::Bounds>::repository_id_, CORBA::TypeCode::repository_id_ };
