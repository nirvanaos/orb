#ifndef NIRVANA_ORB_PROXYFACTORY_C_H_
#define NIRVANA_ORB_PROXYFACTORY_C_H_

#include "ReferenceCounter_c.h"
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
			//! Returns ids of all interfaces.
			const CountedArray <const Char*>* (*interfaces) (Bridge <ProxyFactory>*, EnvironmentBridge*);
			void (*create_server_proxy) (Bridge <ProxyFactory>*, Bridge <Interface>*, BridgeMarshal <ReferenceCounter>*, 
																	 Bridge <Interface>**, BridgeMarshal <DynamicServant>**, EnvironmentBridge*);
			void (*create_client_proxy) (Bridge <ProxyFactory>*, const void*, BridgeMarshal <ReferenceCounter>*,
																	 Bridge <Interface>**, BridgeMarshal <DynamicServant>**, EnvironmentBridge*);
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
	const CountedArray <const Char*>* interfaces ();
	void create_server_proxy (Bridge <Interface>*, ReferenceCounter_ptr, Interface_out, DynamicServant_out);
	void create_client_proxy (const void*, ReferenceCounter_ptr, Interface_out, DynamicServant_out);
};

template <class T>
const CountedArray <const Char*>* Client <T, ProxyFactory>::interfaces ()
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	const CountedArray <const Char*>* _ret = (_b._epv ().epv.interfaces) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ProxyFactory>::create_server_proxy (Bridge <Interface>* servant, ReferenceCounter_ptr refcnt, Interface_out proxy, DynamicServant_out dynamic)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	const CountedArray <const Char*>* _ret = (_b._epv ().epv.create_server_proxy) (&_b, servant, refcnt, proxy, dynamic, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ProxyFactory>::create_client_proxy (const void* address, ReferenceCounter_ptr refcnt, Interface_out proxy, DynamicServant_out dynamic)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	const CountedArray <const Char*>* _ret = (_b._epv ().epv.create_client_proxy) (&_b, address, refcnt, proxy, dynamic, &_env);
	_env.check ();
	return _ret;
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
