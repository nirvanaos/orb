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
#ifndef NIRVANA_ORB_OBJECT_C_H_
#define NIRVANA_ORB_OBJECT_C_H_
#pragma once

#include "Client.h"
#include "basic_types.h"
#include "TypeObject.h"
#include "Sequence.h"
#include "ExceptionList.h"

namespace PortableServer {
class ServantBase;
}

namespace CORBA {

class Object;

typedef Internal::Interface ImplementationDef; // Not defined, unused

class InterfaceDef;
class Context;
class NamedValue;
class NVList;
class Policy;
typedef ::IDL::Sequence <Internal::TypeItf <Policy>::Var> PolicyList;
typedef ULong PolicyType;
typedef ::IDL::Sequence <PolicyType> PolicyTypeSeq;
typedef ULong Flags;
class Request;
typedef ::IDL::Sequence < ::IDL::String> ContextList;
class DomainManager;
typedef ::IDL::Sequence <Internal::TypeItf <DomainManager>::Var> DomainManagersList;
enum class SetOverrideType : Internal::ABI_enum;
class ORB;

namespace Internal {

template <> const Char RepIdOf <Object>::id [] = CORBA_REPOSITORY_ID ("Object");
NIRVANA_BRIDGE_BEGIN (CORBA::Object)
Interface* (*get_interface) (Bridge <Object>*, Interface*);
Type <Boolean>::ABI_ret (*is_a) (Bridge <Object>*, Type <String>::ABI_in, Interface*);
Type <Boolean>::ABI_ret (*non_existent) (Bridge <Object>*, Interface*);
Type <Boolean>::ABI_ret (*is_equivalent) (Bridge <Object>*, Interface*, Interface*);
ULong (*hash) (Bridge <Object>*, ULong maximum, Interface*);
void (*create_request) (Bridge <Object>*, Interface* ctx, Type <String>::ABI_in operation,
	Interface* arg_list, Interface* result, Interface** req, uint32_t req_flags, Interface*);
void (*create_request2) (Bridge <Object>*, Interface* ctx, Type <String>::ABI_in operation,
	Interface* arg_list, Interface* result, Type <Dynamic::ExceptionList>::ABI_in, Type <ContextList>::ABI_in,
	Interface** req, uint32_t req_flags, Interface*);
Interface* (*get_policy) (Bridge <Object>*, uint32_t, Interface*);
Type <DomainManagersList>::ABI_ret (*get_domain_managers) (Bridge <Object>*, Interface*);
Interface* (*set_policy_overrides) (Bridge <Object>*, Type <PolicyList>::ABI_in, ABI_enum, Interface*);
Interface* (*get_client_policy) (Bridge <Object>*, PolicyType, Interface*);
Type <PolicyList>::ABI_ret (*get_policy_overrides) (Bridge <Object>*, Type <PolicyTypeSeq>::ABI_in, Interface*);
Type <Boolean>::ABI_ret (*validate_connection) (Bridge <Object>*, Type <PolicyList>::ABI_out, Interface*);
Type <String>::ABI_ret (*repository_id) (Bridge <Object>*, Interface*);
Interface* (*get_component) (Bridge <Object>*, Interface*);

Interface* (*query_interface) (Bridge <Object>*, Type <String>::ABI_in, Interface*);
Interface* (*get_servant) (Bridge <Object>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, Object> :
	public T
{
public:
	I_ptr <ImplementationDef> _get_implementation ();

#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<InterfaceDef> _get_interface ();

	Boolean _is_a (String_in type_id);
	Boolean _non_existent ();
	Boolean _is_equivalent (I_in <Object> other_object);
	ULong _hash (ULong maximum);
	void _create_request (I_in <Context> ctx, String_in operation, I_in <NVList> arg_list,
		I_in <NamedValue> result, I_out <Request> request, Flags req_flags);
	void _create_request (I_in <Context> ctx, String_in operation, I_in <NVList> arg_list,
		I_in <NamedValue> result, Type <Dynamic::ExceptionList>::C_in exclist, Type <ContextList>::C_in ctxlist,
		I_out <Request> request, Flags req_flags);

#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<Request> _request (String_in operation);

#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<Policy> _get_policy (PolicyType policy_type);

	DomainManagersList _get_domain_managers ();
	
#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<Object> _set_policy_overrides (Type <PolicyList>::C_in policies, SetOverrideType set_or_add);

#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<Policy> _get_client_policy (PolicyType type);

	PolicyList _get_policy_overrides (Type <PolicyTypeSeq>::C_in types);
	Boolean _validate_connection (Type <PolicyList>::C_out inconsistent_policies);
	String _repository_id ();

#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<Object> _get_component ();

#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<ORB> _get_ORB () noexcept;

	/// This method does not increment reference counter
	I_ptr <Interface> _query_interface (String_in type_id);

	/// This method does not increment reference counter
	template <class Itf>
	I_ptr <Itf> _query_interface ()
	{
		return _query_interface (RepIdOf <Itf>::id).template downcast <Itf> ();
	}

	/// Obtain servant from object.
	/// 
	/// Nirvana extension. This operation may be called on the server side for active object.
	/// May be called only from the synchronization context where the servant was created.
	/// Otherwise an exception will thrown.
	/// 
	/// \returns ServantBase reference.
	/// \throws OBJ_ADAPTER(0)      Called in wrong synchronization context.
	/// \throws NO_IMPLEMENT(0)     Wrong object.
	/// \throws OBJECT_NOT_EXIST(1) Object is not active.
#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<PortableServer::ServantBase> _get_servant ();
};

}
}

#endif
