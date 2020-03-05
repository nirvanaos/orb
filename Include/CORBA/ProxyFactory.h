#ifndef NIRVANA_ORB_PROXYFACTORY_H_
#define NIRVANA_ORB_PROXYFACTORY_H_

#include "CORBA.h"
#include "DynamicServant.h"
#include "LocalMarshal.h"

namespace CORBA {
namespace Nirvana {

class ProxyFactory;
typedef I_ptr <ProxyFactory> ProxyFactory_ptr;
typedef I_var <ProxyFactory> ProxyFactory_var;
typedef I_out <ProxyFactory> ProxyFactory_out;

template <class T>
struct CountedArray
{
	const T* p;
	ULong size;
};

class ServerRequest;
class RemoteRequest;

typedef void (*ServerRequestProc) (Interface*, ServerRequest*, EnvironmentBridge*);
typedef void (*LocalRequestProc) (Interface*, void*, EnvironmentBridge*);
typedef void (*RemoteRequestProc) (Interface*, RemoteRequest*, EnvironmentBridge*);

struct Operation
{
	const char* operation;
	LocalRequestProc invoke_local;
	ServerRequestProc invoke_dii;
	RemoteRequestProc invoke_remote;
};

/*
~~~{.idl}
pseudo interface ProxyFactory {
	//! Returns ids of all interfaces.
	const CountedArray <const char*>& interfaces ();
	const CountedArray <const Operation>& operations ();
	Interface create_servant_proxy (Object proxy, Object servant, out DynamicServant deleter);
	Interface create_local_proxy (Object proxy, in LocalObjectRef target, short interface_idx, out DynamicServant deleter);
	Interface create_remote_proxy (Object proxy, out DynamicServant deleter);
};
~~~
*/

BRIDGE_BEGIN (ProxyFactory, CORBA_NIRVANA_REPOSITORY_ID ("ProxyFactory"))
const CountedArray <const char*> interfaces;
const CountedArray <const Operation> operations;
Interface* (*create_servant_proxy) (Bridge <ProxyFactory>*, Interface*,
	Interface*,
	Interface**, EnvironmentBridge*);
Interface* (*create_local_proxy) (Bridge <ProxyFactory>*, Interface*,
	const LocalObjectRef* target,
	UShort interface_idx,
	Interface**, EnvironmentBridge*);
Interface* (*create_remote_proxy) (Bridge <ProxyFactory>*, Interface*,
	Interface**, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ProxyFactory> :
	public T
{
public:
	const CountedArray <const char*>& interfaces ()
	{
		Environment _env;
		return T::_get_bridge (_env).interfaces;
	}

	const CountedArray <const Operation>& operations ()
	{
		Environment _env;
		return T::_get_bridge (_env).operations;
	}

	Interface_var create_servant_proxy (I_in <Object> proxy, I_in <Object> servant, I_out <DynamicServant> deleter);
	Interface_var create_local_proxy (I_in <Object> obj, const LocalObjectRef& target, UShort interface_idx, I_out <DynamicServant> deleter);
	Interface_var create_remote_proxy (I_in <Object> obj, I_out <DynamicServant> deleter);
};

template <class T>
Interface_var Client <T, ProxyFactory>::create_servant_proxy (I_in <Object> proxy, I_in <Object> servant, I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_ret <Interface> _ret = (_b._epv ().epv.create_servant_proxy) (&_b, &proxy, &servant, &deleter, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Interface_var Client <T, ProxyFactory>::create_local_proxy (I_in <Object> obj, const LocalObjectRef& target, UShort interface_idx, I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_ret <Interface> _ret = (_b._epv ().epv.create_local_proxy) (&_b, &obj, &target, interface_idx, &deleter, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Interface_var Client <T, ProxyFactory>::create_remote_proxy (I_in <Object> obj, I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_ret <Interface> _ret = (_b._epv ().epv.create_remote_proxy) (&_b, &obj, &deleter, &_env);
	_env.check ();
	return _ret;
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
