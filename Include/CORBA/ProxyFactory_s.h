#ifndef NIRVANA_ORB_PROXYFACTORY_S_H_
#define NIRVANA_ORB_PROXYFACTORY_S_H_

#include "ProxyFactory.h"
#include "ImplementationPseudo.h"

// ProxyFactory skeleton

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ProxyFactory>
{
public:
	static const typename Bridge <ProxyFactory>::EPV epv_;

protected:
	static const CountedArray <const Char*>* _interfaces (Bridge <ProxyFactory>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).interfaces ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}

	static Interface* _create_servant_proxy (Bridge <ProxyFactory>* obj, Interface* proxy,
		Interface* sync, Interface* servant,
		Interface** deleter, EnvironmentBridge* env)
	{
		try {
			return TypeI <Interface>::VT_ret (S::_implementation (obj).create_servant_proxy (TypeI <Object>::in (proxy), 
				TypeI <::Nirvana::SyncDomainTraits>::in (servant), TypeI <Interface>::in (servant),
				TypeI <DynamicServant>::out (deleter)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}

	static Interface* _create_local_proxy (Bridge <ProxyFactory>* obj, Interface* proxy, 
		const LocalObjectRef* target, UShort interface_idx,
		Interface** deleter, EnvironmentBridge* env)
	{
		try {
			return TypeI <Interface>::VT_ret (S::_implementation (obj).create_local_proxy (TypeI <Object>::in (proxy), 
				*target, interface_idx,
				TypeI <DynamicServant>::out (deleter)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}
};

template <class S>
const Bridge <ProxyFactory>::EPV Skeleton <S, ProxyFactory>::epv_ = {
	{ // header
		Bridge <ProxyFactory>::interface_id_,
		S::template __duplicate <ProxyFactory>,
		S::template __release <ProxyFactory>
	},
	{ // epv
		S::_interfaces,
		S::_create_servant_proxy,
		S::_create_local_proxy
	}
};

template <class S>
class ServantStatic <S, ProxyFactory> :
	public ImplementationPseudoStatic <S, ProxyFactory>
{};

}
}

#endif
