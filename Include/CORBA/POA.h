// Nirvana project
// Object Request Broker
// PortableServer::POA interface
#ifndef NIRVANA_ORB_POA_H_
#define NIRVANA_ORB_POA_H_

#include "ServantBase.h"
#include "TypeCode.h"

namespace CORBA {
namespace Nirvana {

template <class T>
String Client <T, PortableServer::POA>::activate_object (I_in <PortableServer::ServantBase> servant)
{
	Environment _env;
	Bridge <PortableServer::POA>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.activate_object) (&_b, &servant, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, PortableServer::POA>::deactivate_object (String_in oid)
{
	Environment _env;
	Bridge <PortableServer::POA>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.deactivate_object) (&_b, &oid, &_env);
	_env.check ();
}

}
}

#endif
