#ifndef NIRVANA_ORB_PROXYFACTORY_S_H_
#define NIRVANA_ORB_PROXYFACTORY_S_H_

#include "ProxyFactory.h"
#include "../ImplementationPseudoStatic.h"

// ProxyFactory skeleton

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ProxyFactory>
{
public:
	static const typename Bridge <ProxyFactory>::EPV epv_;

protected:
	static Interface* _create_proxy (Bridge <ProxyFactory>* obj,
		Interface* target, UShort interface_idx,
		Interface** deleter, EnvironmentBridge* env)
	{
		try {
			return TypeI <Interface>::VT_ret (S::_implementation (obj).create_proxy (
				TypeI <IOReference>::in (target), interface_idx,
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
		Bridge <ProxyFactory>::repository_id_,
		S::template __duplicate <ProxyFactory>,
		S::template __release <ProxyFactory>
	},
	{ // epv
		&S::metadata_,
		S::_create_proxy
	}
};

/// Proxy factory implements ProxyFactory and TypeCode interfaces.
template <class S>
class ServantStatic <S, ProxyFactory> :
	public ImplementationPseudoStatic <S, ProxyFactory, TypeCode, AbstractBase>
{
public:
	Interface_ptr _query_interface (const String& id)
	{
		return FindInterface <ProxyFactory, TypeCode>::find (*(S*)0, id);
	}
};

}
}

#endif
