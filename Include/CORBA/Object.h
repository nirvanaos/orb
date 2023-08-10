/// \file
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
#ifndef NIRVANA_ORB_OBJECT_H_
#define NIRVANA_ORB_OBJECT_H_
#pragma once

#include "Object_c.h"
#include "TypeObject.h"
#include "TypeAlias.h"
#include "UserException.h"
#include "Any.h"
#include "EnvironmentEx.h"
#include "core_objects.h"

// IDL generated headers
#include "IORequest.h"
#include "Context.h"
#include "Request.h"
#include "Policy.h"

// ServantBase
#include "ServantBase.h"

namespace CORBA {

#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <Object> Object_ptr;
typedef Internal::I_var <Object> Object_var;
#endif

namespace Internal {

template <class T>
Internal::I_ptr <ImplementationDef> Client <T, Object>::_get_implementation ()
{
	return nullptr; // Compatibility stub
}

template <class T>
#ifdef LEGACY_CORBA_CPP
I_ptr
#else
I_ref
#endif
<InterfaceDef> Client <T, Object>::_get_interface ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret ((_b._epv ().epv.get_interface) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_a (String_in type_id)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret ((_b._epv ().epv.is_a) (&_b, &type_id, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_non_existent ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret ((_b._epv ().epv.non_existent) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_equivalent (I_in <Object> other_object)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret ((_b._epv ().epv.is_equivalent) (&_b, &other_object, &_env));
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, Object>::_hash (ULong maximum)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.hash) (&_b, maximum, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, Object>::_create_request (I_in <Context> ctx, String_in operation,
	I_in <NVList> arg_list, I_in <NamedValue> result, I_out <Request> request, Flags req_flags)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.create_request) (&_b, &ctx, &operation, &arg_list, &result, &request, req_flags, &_env);
	_env.check ();
}

template <class T>
void Client <T, Object>::_create_request (I_in <Context> ctx, String_in operation,
	I_in <NVList> arg_list, I_in <NamedValue> result, Type <ExceptionList>::C_in exclist,
	Type <ContextList>::C_in ctxlist, I_out <Request> request, Flags req_flags)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.create_request2) (&_b, &ctx, &operation, &arg_list, &result, &exclist, &ctxlist,
		&request, req_flags, &_env);
	_env.check ();
}

template <class T>
I_ref <Request> Client <T, Object>::_request (String_in operation)
{
	I_ref <Request> rq;
	_create_request (Context::_nil (), operation, NVList::_nil (), NamedValue::_nil (), rq, 0);
	return rq;
}

template <class T>
I_ref <Policy> Client <T, Object>::_get_policy (PolicyType policy_type)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	I_ret <Policy> _ret ((_b._epv ().epv.get_policy) (&_b, policy_type, &_env));
	_env.check ();
	return _ret;
}

template <class T>
DomainManagersList Client <T, Object>::_get_domain_managers ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <DomainManagersList>::C_ret _ret ((_b._epv ().epv.get_domain_managers) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
I_ref <Object> Client <T, Object>::_set_policy_overrides (Type <PolicyList>::C_in policies,
	SetOverrideType set_or_add)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Object>::C_ret _ret ((_b._epv ().epv.set_policy_overrides) (&_b, &policies, (ABI_enum)set_or_add, &_env));
	_env.check ();
	return _ret;
}

template <class T>
I_ref <Policy> Client <T, Object>::_get_client_policy (PolicyType type)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Policy>::C_ret _ret ((_b._epv ().epv.get_client_policy) (&_b, type, &_env));
	_env.check ();
	return _ret;
}

template <class T>
PolicyList Client <T, Object>::_get_policy_overrides (Type <PolicyTypeSeq>::C_in types)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <PolicyList>::C_ret _ret ((_b._epv ().epv.get_policy_overrides) (&_b, &types, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_validate_connection (Type <PolicyList>::C_out inconsistent_policies)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret ((_b._epv ().epv.validate_connection) (&_b, &inconsistent_policies, &_env));
	_env.check ();
	return _ret;
}

template <class T>
String Client <T, Object>::_repository_id ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret ((_b._epv ().epv.repository_id) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
I_ref <Object> Client <T, Object>::_get_component ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Object>::C_ret _ret ((_b._epv ().epv.get_component) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
I_ref <ORB> Client <T, Object>::_get_ORB () noexcept
{
	return I_ptr <ORB> (g_ORB);
}

template <class T>
I_ptr <Interface> Client <T, Object>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret ((_b._epv ().epv.query_interface) (&_b, &type_id, &_env));
	_env.check ();
	return _ret;
}

template <class T>
I_ref <PortableServer::ServantBase> Client <T, Object>::_get_servant ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	I_ret <PortableServer::ServantBase> _ret ((_b._epv ().epv.get_servant) (&_b, &_env));
	_env.check ();
	return _ret;
}

}

class Object : 
	public Internal::ClientInterfacePrimary <Object>
{
public:
#ifndef LEGACY_CORBA_CPP
	static Internal::I_ref <Object> _narrow (Object::_ptr_type obj)
	{
		return obj;
	}
#else
	NIRVANA_NODISCARD static Object::_ptr_type _narrow (Object::_ptr_type obj)
	{
		return ClientInterfacePrimary <Object>::_duplicate (obj);
	}
#endif
};

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, Object> :
	public Client <ClientBase <Primary, Object>, Object>
{
public:
#ifndef LEGACY_CORBA_CPP
	static I_ref <Primary> _narrow (Object::_ptr_type obj)
	{
		if (obj)
			return obj->_query_interface <Primary> ();
		return nullptr;
	}
#else
	NIRVANA_NODISCARD static I_ptr <Primary> _narrow (Object::_ptr_type obj)
	{
		if (obj)
			return ClientInterfacePrimary <Primary>::_duplicate (obj->_query_interface <Primary> ());
		return nullptr;
	}
#endif
};

}
}

#endif
