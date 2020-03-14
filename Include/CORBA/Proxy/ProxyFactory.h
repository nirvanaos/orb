#ifndef NIRVANA_ORB_PROXYFACTORY_H_
#define NIRVANA_ORB_PROXYFACTORY_H_

#include "../CORBA.h"
#include "../DynamicServant.h"
#include "PlatformObjRef.h"
#include "PlatformRequest.h"

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

typedef PlatformMarshal_var (*PlatformRequestProc) (Interface*, 
	PlatformRequest_ptr call,
	::Nirvana::Pointer* in_params, 
	PlatformUnmarshal_var unmarshaler, // Unmarshaler should be released after the unmarshal completion.
	::Nirvana::Pointer* out_params);

typedef void (*ServerRequestProc) (Interface*, ServerRequest*, EnvironmentBridge*);
typedef void (*RemoteRequestProc) (Interface*, RemoteRequest*, EnvironmentBridge*);

struct Operation
{
	const char* name;
	PlatformRequestProc invoke_platform;
	ServerRequestProc invoke_dii;
	RemoteRequestProc invoke_remote;
};

/*
~~~{.idl}
pseudo interface ProxyFactory {
	//! Returns ids of all interfaces.
	const CountedArray <const char*>& interfaces ();
	const CountedArray <const Operation>& operations ();
	Interface create_platform_proxy (PlatformObjRef target, short interface_idx, AbstractBase bases, out DynamicServant deleter);
	Interface create_remote_proxy (RemoteObjRef target, AbstractBase bases, out DynamicServant deleter);
};
~~~
*/

BRIDGE_BEGIN (ProxyFactory, CORBA_NIRVANA_REPOSITORY_ID ("ProxyFactory"))
const CountedArray <const char*> interfaces;
const CountedArray <const Operation> operations;
Interface* (*create_platform_proxy) (Bridge <ProxyFactory>*, 
	Interface*, UShort interface_idx,
	Interface*, Interface**, EnvironmentBridge*);
Interface* (*create_remote_proxy) (Bridge <ProxyFactory>*,
	Interface*,
	Interface*, Interface**, EnvironmentBridge*);
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

	Interface_ptr create_platform_proxy (
		I_in <PlatformObjRef> target, UShort interface_idx, 
		I_in <AbstractBase> bases, I_out <DynamicServant> deleter);
/*
	Interface_ptr create_remote_proxy (I_in <RemoteObjRef> target,
		I_in <AbstractBase> bases, I_out <DynamicServant> deleter);
*/
};

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_platform_proxy (
	I_in <PlatformObjRef> target, UShort interface_idx,
	I_in <AbstractBase> bases, I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.create_platform_proxy) (&_b, &target, interface_idx, bases, &deleter, &_env);
	_env.check ();
	return _ret;
}
/*
template <class T>
Interface_ptr Client <T, ProxyFactory>::create_remote_proxy (I_in <RemoteObjRef> target, I_in <AbstractBase> bases, I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.create_remote_proxy) (&_b, &target, &deleter, &_env);
	_env.check ();
	return _ret;
}
*/
class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
