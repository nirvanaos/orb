// Nirvana project
// Object Request Broker
// PortableServer::POA interface
#ifndef NIRVANA_ORB_POA_C_H_
#define NIRVANA_ORB_POA_C_H_

#include "ServantBase_c.h"

namespace CORBA {
namespace Nirvana {

class ServantBase;

template <>
class Bridge < ::PortableServer::POA> :
	public BridgeMarshal < ::PortableServer::POA>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
		}
		base;

		struct
		{
			const Char* (*activate_object) (Bridge < ::PortableServer::POA>*, BridgeMarshal <ServantBase>*, EnvironmentBridge*);
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
		BridgeMarshal < ::PortableServer::POA> (epv.interface)
	{}
};

template <class T>
class Client <T, ::PortableServer::POA> :
	public T
{
public:
	const Char* activate_object (ServantBase_ptr servant);
};

template <class T>
const Char* Client <T, ::PortableServer::POA>::activate_object (ServantBase_ptr servant)
{
	Environment _env;
	Bridge < ::PortableServer::POA>& _b (T::_get_bridge (_env));
	const Char* _ret = (_b._epv ().epv.activate_object) (&_b, servant, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace PortableServer {

class POA :	public ::CORBA::Nirvana::ClientInterface <POA, ::CORBA::Object>
{};

}

#endif
