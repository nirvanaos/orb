/// \file
/// PortableServer::POA interface
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
#ifndef NIRVANA_ORB_POA_H_
#define NIRVANA_ORB_POA_H_

#include "ServantBase.h"

namespace PortableServer {

class POA;
#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Internal::I_ptr <POA> POA_ptr;
typedef ::CORBA::Internal::I_var <POA> POA_var;
#endif

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_POA;

}

namespace CORBA {
namespace Internal {

template <>
struct Type < ::PortableServer::POA> : TypeLocalObject < ::PortableServer::POA>
{
	static I_ptr <TypeCode> type_code ()
	{
		return ::PortableServer::_tc_POA;
	}
};

template <>
struct Definitions <PortableServer::POA>
{
	static const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_ServantAlreadyActive;

	class ServantAlreadyActive : public ::CORBA::UserException
	{
	public:
		NIRVANA_EXCEPTION_DCL (ServantAlreadyActive);
	};

	static const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_ObjectNotActive;

	class ObjectNotActive : public ::CORBA::UserException
	{
	public:
		NIRVANA_EXCEPTION_DCL (ObjectNotActive);
	};
};

template <>
const Char RepIdOf <Definitions <PortableServer::POA>::ServantAlreadyActive>::repository_id_ [] = PORTABLESERVER_REPOSITORY_ID ("POA/ServantAlreadyActive");

template <>
const Char RepIdOf <Definitions <PortableServer::POA>::ObjectNotActive>::repository_id_ [] = PORTABLESERVER_REPOSITORY_ID ("POA/ObjectNotActive");

NIRVANA_BRIDGE_BEGIN (PortableServer::POA, PORTABLESERVER_REPOSITORY_ID ("POA"))
NIRVANA_BASE_ENTRY (CORBA::Object, CORBA_Object)
NIRVANA_BRIDGE_EPV
Type <String>::ABI_ret (*activate_object) (Bridge <PortableServer::POA>*, Interface*, Interface*);
void (*deactivate_object) (Bridge <PortableServer::POA>*, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, PortableServer::POA> :
	public T,
	public Definitions <PortableServer::POA>
{
public:
	String activate_object (I_in <PortableServer::ServantBase> servant);
	void deactivate_object (String_in oid);
};

template <class T>
String Client <T, PortableServer::POA>::activate_object (I_in <PortableServer::ServantBase> servant)
{
	Environment _env;
	Bridge <PortableServer::POA>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.activate_object) (&_b, &servant, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, PortableServer::POA>::deactivate_object (String_in oid)
{
	Environment _env;
	Bridge <PortableServer::POA>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.deactivate_object) (&_b, &oid, &_env);
	_env.check ();
}

}
}

namespace PortableServer {

class POA : public ::CORBA::Internal::ClientInterface <POA, CORBA::Object>
{
public:
	using CORBA::Internal::Definitions <POA>::_tc_ServantAlreadyActive;
	using CORBA::Internal::Definitions <POA>::ServantAlreadyActive;

	using CORBA::Internal::Definitions <POA>::_tc_ObjectNotActive;
	using CORBA::Internal::Definitions <POA>::ObjectNotActive;
};

}

#endif
