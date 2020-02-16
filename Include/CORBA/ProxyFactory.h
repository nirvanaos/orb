#ifndef NIRVANA_ORB_PROXYFACTORY_H_
#define NIRVANA_ORB_PROXYFACTORY_H_

#include "Object.h"
#include "DynamicServant.h"

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
pseudo interface ProxyFactory {
	//! Returns ids of all interfaces.
	const CountedArray <const char*>& interfaces ();
	const CountedArray <const Operation>& operations ();
	Interface create_server_proxy (ProxyManager pm, Interface itf, out DynamicServant deleter);
	Interface create_local_proxy (ProxyManager pm, uint16_t interface_idx, out DynamicServant deleter);
	Interface create_remote_proxy (ProxyManager pm, out DynamicServant deleter);
};
*/

BRIDGE_BEGIN (ProxyFactory)
const CountedArray <const char*> interfaces;
const CountedArray <const Operation> operations;
Interface* (*create_server_proxy) (Bridge <ProxyFactory>*, Interface*,
	Interface*,
	Interface**, EnvironmentBridge*);
Interface* (*create_local_proxy) (Bridge <ProxyFactory>*, Interface*,
	uint16_t interface_idx,
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

	Interface_ptr create_server_proxy (Object_ptr obj, Interface_ptr servant, DynamicServant_out deleter);
	Interface_ptr create_local_proxy (Object_ptr obj, uint16_t interface_idx, DynamicServant_out deleter);
	Interface_ptr create_remote_proxy (Object_ptr obj, DynamicServant_out deleter);
};

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_server_proxy (Object_ptr obj, Interface_ptr servant, DynamicServant_out deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_ret <Interface> _ret = (_b._epv ().epv.create_server_proxy) (&_b, obj, servant, &deleter, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_local_proxy (Object_ptr obj, uint16_t interface_idx, DynamicServant_out deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_ret <Interface> _ret = (_b._epv ().epv.create_local_proxy) (&_b, obj, interface_idx, &deleter, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_remote_proxy (Object_ptr obj, DynamicServant_out deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_ret <Interface> _ret = (_b._epv ().epv.create_remote_proxy) (&_b, obj, &deleter, &_env);
	_env.check ();
	return _ret;
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
