// Nirvana project
// Object Request Broker
// PortableServer::POA interface
#ifndef NIRVANA_ORB_POA_H_
#define NIRVANA_ORB_POA_H_

#include "ServantBase.h"

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (PortableServer::POA)
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
const Char* (*activate_object) (Bridge <PortableServer::POA>*, Interface*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, PortableServer::POA> :
	public T
{
public:
	const Char* activate_object (PortableServer::Servant servant);
};

template <class T>
const Char* Client <T, PortableServer::POA>::activate_object (PortableServer::Servant servant)
{
	Environment _env;
	Bridge <PortableServer::POA>& _b (T::_get_bridge (_env));
	const Char* _ret = (_b._epv ().epv.activate_object) (&_b, servant, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace PortableServer {

class POA :	public CORBA::Nirvana::ClientInterface <POA, CORBA::Object>
{};

}

#endif
