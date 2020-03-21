#ifndef NIRVANA_ORB_PROXYFACTORY_H_
#define NIRVANA_ORB_PROXYFACTORY_H_

#include "InterfaceMetadata.h"
#include "PlatformObjRef.h"
#include "../DynamicServant.h"

namespace CORBA {
namespace Nirvana {

class ProxyFactory;
typedef I_ptr <ProxyFactory> ProxyFactory_ptr;
typedef I_var <ProxyFactory> ProxyFactory_var;
typedef I_out <ProxyFactory> ProxyFactory_out;

/*
~~~{.idl}
pseudo interface ProxyFactory {
	const InterfaceMetadata* metadata ();
	Interface create_platform_proxy (PlatformObjRef target, short interface_idx, out DynamicServant deleter);
	Interface create_remote_proxy (RemoteObjRef target, out DynamicServant deleter);
};
~~~
*/

BRIDGE_BEGIN (ProxyFactory, CORBA_NIRVANA_REPOSITORY_ID ("ProxyFactory"))
const InterfaceMetadata* metadata;
Interface* (*create_platform_proxy) (Bridge <ProxyFactory>*, 
	Interface*, UShort interface_idx,
	Interface**, EnvironmentBridge*);
Interface* (*create_remote_proxy) (Bridge <ProxyFactory>*,
	Interface*,
	Interface**, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ProxyFactory> :
	public T
{
public:
	const InterfaceMetadata* metadata ()
	{
		Environment _env;
		return T::_get_bridge (_env).metadata;
	}

	Interface_ptr create_platform_proxy (
		I_in <PlatformObjRef> target, UShort interface_idx, 
		I_out <DynamicServant> deleter);
/*
	Interface_ptr create_remote_proxy (I_in <RemoteObjRef> target,
		I_in <AbstractBase> bases, I_out <DynamicServant> deleter);
*/
};

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_platform_proxy (
	I_in <PlatformObjRef> target, UShort interface_idx,
	I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.create_platform_proxy) (&_b, &target, interface_idx, &deleter, &_env);
	_env.check ();
	return _ret;
}
/*
template <class T>
Interface_ptr Client <T, ProxyFactory>::create_remote_proxy (I_in <RemoteObjRef> target, I_out <DynamicServant> deleter)
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
