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
#ifndef NIRVANA_ORB_OBJECT_S_H_
#define NIRVANA_ORB_OBJECT_S_H_
#pragma once

#include "Object.h"

namespace CORBA {
namespace Internal {

//! Object skeleton

template <class S>
class Skeleton <S, Object>
{
public:
	static const typename Bridge <Object>::EPV epv_;

protected:
	static Interface* __get_interface (Bridge <Object>* obj, Interface* env)
	{
		try {
			return Type <InterfaceDef>::ret (S::_implementation (obj)._get_interface ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <InterfaceDef>::ret ();
	}

	static Type <Boolean>::ABI_ret __is_a (Bridge <Object>* obj, Type <String>::ABI_in type_id, Interface* env)
	{
		try {
			return S::_implementation (obj)._is_a (Type <String>::in (type_id));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return false;
	}

	static Type <Boolean>::ABI_ret __non_existent (Bridge <Object>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return false;
	}

	static Type <Boolean>::ABI_ret __is_equivalent (Bridge <Object>* obj, Interface* other_object, Interface* env)
	{
		try {
			return S::_implementation (obj)._is_equivalent (Type <Object>::in (other_object));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return false;
	}

	static ULong __hash (Bridge <Object>* obj, ULong maximum, Interface* env)
	{
		try {
			return S::_implementation (obj)._hash (maximum);
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static void __create_request (Bridge <Object>* obj, Interface* ctx, Type <String>::ABI_in operation,
		Interface* arg_list, Interface* result, Interface** request, Flags req_flags, Interface* env)
	{
		try {
			return S::_implementation (obj)._create_request (Type <Context>::in (ctx), Type <String>::in (operation),
				Type <NVList>::in (arg_list), Type <NamedValue>::in (result), Type <CORBA::Request>::out (request), req_flags);
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static void __create_request2 (Bridge <Object>* obj, Interface* ctx, Type <String>::ABI_in operation,
		Interface* arg_list, Interface* result, Type <Dynamic::ExceptionList>::ABI_in exclist, Type <ContextList>::ABI_in ctxlist,
		Interface** request, uint32_t req_flags, Interface* env)
	{
		try {
			return S::_implementation (obj)._create_request (Type <Context>::in (ctx), Type <String>::in (operation),
				Type <NVList>::in (arg_list), Type <NamedValue>::in (result), Type <Dynamic::ExceptionList>::in (exclist),
				Type <ContextList>::in (ctxlist), Type <CORBA::Request>::out (request), req_flags);
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static Interface* __get_policy (Bridge <Object>* obj, uint32_t policy_type, Interface* env)
	{
		try {
			return Type <Policy>::ret (S::_implementation (obj)._get_policy (policy_type));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <Policy>::ret ();
	}

	static Type <DomainManagersList>::ABI_ret __get_domain_managers (Bridge <Object>* obj, Interface* env)
	{
		try {
			return Type <DomainManagersList>::ret (S::_implementation (obj)._get_domain_managers ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <DomainManagersList>::ret ();
	}

	static Interface* __set_policy_overrides (Bridge <Object>* obj, Type <PolicyList>::ABI_in policies,
		ABI_enum set_or_add, Interface* env)
	{
		try {
			return Type <Object>::ret (S::_implementation (obj)._set_policy_overrides (
				Type <PolicyList>::in (policies), Type <SetOverrideType>::in (set_or_add)));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <Object>::ret ();
	}

	static Interface* __get_client_policy (Bridge <Object>* obj, PolicyType type, Interface* env)
	{
		try {
			return Type <Policy>::ret (S::_implementation (obj)._get_client_policy (type));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <Policy>::ret ();
	}

	static Type <PolicyList>::ABI_ret __get_policy_overrides (Bridge <Object>* obj,
		Type <PolicyTypeSeq>::ABI_in types, Interface* env)
	{
		try {
			return Type <PolicyList>::ret (S::_implementation (obj)._get_policy_overrides (
				Type <PolicyTypeSeq>::in (types)));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <PolicyList>::ret ();
	}

	static Type <Boolean>::ABI_ret __validate_connection (Bridge <Object>* obj,
		Type <PolicyList>::ABI_out inconsistent_policies, Interface* env)
	{
		try {
			return Type <Boolean>::ret (S::_implementation (obj)._validate_connection (
				Type <PolicyList>::out (inconsistent_policies)));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <Boolean>::ret ();
	}

	static Type <String>::ABI_ret __repository_id (Bridge <Object>* obj, Interface* env)
	{
		try {
			return Type <String>::ret (S::_implementation (obj)._repository_id ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <String>::ret ();
	}

	static Interface* __get_component (Bridge <Object>* obj, Interface* env)
	{
		try {
			return Type <Object>::ret (S::_implementation (obj)._get_component ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <Object>::ret ();
	}

	// Internals
	static Interface* __query_interface (Bridge <Object>* base, Type <String>::ABI_in id, Interface* env)
	{
		try {
			return Type <Interface>::VT_ret (S::_implementation (base)._query_interface (Type <String>::in (id)));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <Interface>::VT_ret ();
	}

	static Interface* __get_servant (Bridge <Object>* base, Interface* env)
	{
		try {
			return Type <PortableServer::ServantBase>::ret (S::_implementation (base)._get_servant ());
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <PortableServer::ServantBase>::ret ();
	}

};

template <class S>
const Bridge <Object>::EPV Skeleton <S, Object>::epv_ = {
	{ // header
		RepIdOf <Object>::id,
		S::template __duplicate <Object>,
		S::template __release <Object>
	},
	{ // epv
		S::__get_interface,
		S::__is_a,
		S::__non_existent,
		S::__is_equivalent,
		S::__hash,
		S::__create_request,
		S::__create_request2,
		S::__get_policy,
		S::__get_domain_managers,
		S::__set_policy_overrides,
		S::__get_client_policy,
		S::__get_policy_overrides,
		S::__validate_connection,
		S::__repository_id,
		S::__get_component,
		
		S::__query_interface,
		S::__get_servant
	}
};

}
}

#endif
