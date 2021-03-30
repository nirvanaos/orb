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
#ifndef NIRVANA_ORB_POA_C_H_
#define NIRVANA_ORB_POA_C_H_

#include "Object.h"
#include "ReferenceCounter.h"
#include "String.h"
#include "UserException.h"

namespace PortableServer {

class POA;
typedef ::CORBA::Nirvana::I_ptr <POA> POA_ptr;
typedef ::CORBA::Nirvana::I_var <POA> POA_var;
typedef ::CORBA::Nirvana::I_out <POA> POA_out;

}

namespace CORBA {
namespace Nirvana {

template <> class I_ptr <PortableServer::ServantBase>;

template <>
struct Definitions <PortableServer::POA>
{
	static const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_ServantAlreadyActive;

	class ServantAlreadyActive : public ::CORBA::UserException
	{
	public:
		DECLARE_EXCEPTION (ServantAlreadyActive);
	};

	static const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_ObjectNotActive;

	class ObjectNotActive : public ::CORBA::UserException
	{
	public:
		DECLARE_EXCEPTION (ObjectNotActive);
	};
};

BRIDGE_BEGIN (PortableServer::POA, PORTABLESERVER_REPOSITORY_ID ("POA"))
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Type <String>::ABI_ret (*activate_object) (Bridge <PortableServer::POA>*, Interface*, Interface*);
void (*deactivate_object) (Bridge <PortableServer::POA>*, Type <String>::ABI_in, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, PortableServer::POA> :
	public T,
	public Definitions <PortableServer::POA>
{
public:
	String activate_object (I_in <PortableServer::ServantBase> servant);
	void deactivate_object (String_in oid);
};

}
}

namespace PortableServer {

typedef ::CORBA::Nirvana::I_ptr <ServantBase> Servant;

class POA : public CORBA::Nirvana::ClientInterface <POA, CORBA::Object>
{
public:
	using CORBA::Nirvana::Definitions <POA>::_tc_ServantAlreadyActive;
	using CORBA::Nirvana::Definitions <POA>::ServantAlreadyActive;

	using CORBA::Nirvana::Definitions <POA>::_tc_ObjectNotActive;
	using CORBA::Nirvana::Definitions <POA>::ObjectNotActive;
};

}

#endif
