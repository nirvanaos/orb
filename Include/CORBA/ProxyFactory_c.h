#ifndef NIRVANA_ORB_PROXYFACTORY_C_H_
#define NIRVANA_ORB_PROXYFACTORY_C_H_

#include "Interface_c.h"

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
			Bridge <Interface>* (*create_server_proxy) (Bridge <ProxyFactory>*, Bridge <Interface>*, EnvironmentBridge*);
			Bridge <Interface>* (*create_client_proxy) (Bridge <ProxyFactory>*, const void*, EnvironmentBridge*);
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
	Bridge <Interface>* create_server_proxy (Bridge <Interface>*);
	Bridge <Interface>* create_client_proxy (const void*);
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
Bridge <Interface>* Client <T, ProxyFactory>::create_server_proxy (Bridge <Interface>* servant)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	const CountedArray <const Char*>* _ret = (_b._epv ().epv.create_server_proxy) (&_b, servant, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Bridge <Interface>* Client <T, ProxyFactory>::create_client_proxy (const void* address)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	const CountedArray <const Char*>* _ret = (_b._epv ().epv.create_client_proxy) (&_b, address, &_env);
	_env.check ();
	return _ret;
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
