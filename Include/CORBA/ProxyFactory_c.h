#ifndef NIRVANA_ORB_PROXYFACTORY_C_H_
#define NIRVANA_ORB_PROXYFACTORY_C_H_

#include "Object_c.h"
#include "DynamicServant_c.h"

namespace CORBA {
namespace Nirvana {

class ProxyFactory;
typedef T_ptr <ProxyFactory> ProxyFactory_ptr;
typedef T_var <ProxyFactory> ProxyFactory_var;
typedef T_out <ProxyFactory> ProxyFactory_out;

template <class T>
struct CountedArray
{
	const T* p;
	ULong size;
};

class ServerRequest;
class RemoteRequest;

typedef void (*ServerRequestProc) (Bridge <Interface>*, ServerRequest*, EnvironmentBridge*);
typedef void (*LocalRequestProc) (Bridge <Interface>*, void*, EnvironmentBridge*);
typedef void (*RemoteRequestProc) (Bridge <Interface>*, RemoteRequest*, EnvironmentBridge*);

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

template <>
class Bridge <ProxyFactory> :
	public BridgeMarshal <ProxyFactory>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			const CountedArray <const char*> interfaces;
			const CountedArray <const Operation> operations;
			BridgeMarshal <Interface>* (*create_server_proxy) (Bridge <ProxyFactory>*, BridgeMarshal <Object>*,
				BridgeMarshal <Interface>*,
				BridgeMarshal <DynamicServant>**, EnvironmentBridge*);
			BridgeMarshal <Interface>* (*create_local_proxy) (Bridge <ProxyFactory>*, BridgeMarshal <Object>*,
				uint16_t interface_idx,
				BridgeMarshal <DynamicServant>**, EnvironmentBridge*);
			BridgeMarshal <Interface>* (*create_remote_proxy) (Bridge <ProxyFactory>*, BridgeMarshal <Object>*,
				BridgeMarshal <DynamicServant>**, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <ProxyFactory> (epv.interface)
	{}
};

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
	Interface_var _ret = (_b._epv ().epv.create_server_proxy) (&_b, obj, servant, deleter, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_local_proxy (Object_ptr obj, uint16_t interface_idx, DynamicServant_out deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	Interface_var _ret = (_b._epv ().epv.create_local_proxy) (&_b, obj, interface_idx, deleter, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_remote_proxy (Object_ptr obj, DynamicServant_out deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	Interface_var _ret = (_b._epv ().epv.create_remote_proxy) (&_b, obj, deleter, &_env);
	_env.check ();
	return _ret._retn ();
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
