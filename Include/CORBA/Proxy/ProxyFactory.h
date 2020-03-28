#ifndef NIRVANA_ORB_PROXYFACTORY_H_
#define NIRVANA_ORB_PROXYFACTORY_H_

#include "InterfaceMetadata.h"
#include "IOReference.h"
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
	Interface create_proxy (IOReference target, short interface_idx, out DynamicServant deleter);
};
~~~
*/

BRIDGE_BEGIN (ProxyFactory, CORBA_NIRVANA_REPOSITORY_ID ("ProxyFactory"))
const InterfaceMetadata* metadata;
Interface* (*create_proxy) (Bridge <ProxyFactory>*, 
	Interface*, UShort interface_idx,
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
		return T::_get_bridge (_env)._epv ().epv.metadata;
	}

	Interface_ptr create_proxy (
		I_in <IOReference> target, UShort interface_idx, 
		I_out <DynamicServant> deleter);
};

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_proxy (
	I_in <IOReference> target, UShort interface_idx,
	I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.create_proxy) (&_b, &target, interface_idx, &deleter, &_env);
	_env.check ();
	return _ret;
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
