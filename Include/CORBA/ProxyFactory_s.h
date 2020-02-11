#ifndef NIRVANA_ORB_PROXYFACTORY_S_H_
#define NIRVANA_ORB_PROXYFACTORY_S_H_

#include "ProxyFactory.h"
#include "Interface.h"

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
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	static Bridge <Interface>* _create_server_proxy (Bridge <ProxyFactory>* obj, Bridge <Interface>* servant, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_server_proxy (servant);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	static Bridge <Interface>* _create_client_proxy (Bridge <ProxyFactory>* obj, const void* address, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_client_proxy (address);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}
};

template <class S>
const Bridge <ProxyFactory>::EPV Skeleton <S, ProxyFactory>::epv_ = {
	{ // interface
		Bridge <ProxyFactory>::interface_id_,
		S::template __duplicate <ProxyFactory>,
		S::template __release <ProxyFactory>
	},
	{ // epv
		S::_interfaces,
		S::_create_server_proxy,
		S::_create_client_proxy
	}
};

template <class S>
class ServantStatic <S, ProxyFactory> :
	public ImplementationStaticPseudo <S, ProxyFactory>
{};

}
}

#endif
