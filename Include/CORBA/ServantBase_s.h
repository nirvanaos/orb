#ifndef NIRVANA_ORB_SERVANTBASE_S_H_
#define NIRVANA_ORB_SERVANTBASE_S_H_

#include "ServantBase.h"
#include "TypeInterface.h"

namespace CORBA {
namespace Nirvana {

// ServantBase skeleton

template <class S>
class Skeleton <S, ::PortableServer::ServantBase>
{
public:
	static const typename Bridge <::PortableServer::ServantBase>::EPV epv_;

protected:
	static Interface* __default_POA (Bridge <::PortableServer::ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return TypeI <::PortableServer::POA>::ret (S::_implementation (obj)._default_POA ());
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Interface* __get_interface (Bridge <::PortableServer::ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return TypeI <InterfaceDef>::ret (S::_implementation (obj)._get_interface ());
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static ABI_boolean __is_a (Bridge <::PortableServer::ServantBase>* obj, ABI_in <String> type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_a (Type <String>::in (type_id));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static ABI_boolean __non_existent (Bridge <::PortableServer::ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}

	static Interface* ___core_servant (Bridge <::PortableServer::ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).__core_servant ();
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <::PortableServer::ServantBase>::EPV Skeleton <S, ::PortableServer::ServantBase>::epv_ = {
	{ // header
		Bridge <::PortableServer::ServantBase>::interface_id_,
		S::template __duplicate <::PortableServer::ServantBase>,
		S::template __release <::PortableServer::ServantBase>
	},
	{ // base
		S::template _wide < ::CORBA::AbstractBase, ::PortableServer::ServantBase>,
		S::template _wide <ReferenceCounter, ::PortableServer::ServantBase>
	},
	{ // epv
		S::__default_POA,
		S::__get_interface,
		S::__is_a,
		S::__non_existent,
		S::___core_servant
	}
};

}
}

#endif
