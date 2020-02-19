// Nirvana project
// Object Request Broker
// PortableServer::POA skeleton
#ifndef NIRVANA_ORB_POA_S_H_
#define NIRVANA_ORB_POA_S_H_

#include "POA.h"
#include "Object_s.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, PortableServer::POA>
{
public:
	static const typename Bridge <PortableServer::POA>::EPV epv_;

protected:
	static const Char* _activate_object (Bridge <PortableServer::POA>* obj, Interface* servant, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).activate_object (TypeI <ServantBase>::in (servant));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <PortableServer::POA>::EPV Skeleton <S, PortableServer::POA>::epv_ = {
	{ // interface
		Bridge <PortableServer::POA>::interface_id_,
		S::template __duplicate <PortableServer::POA>,
		S::template __release <PortableServer::POA>
	},
	{ // base
		S::template _wide <Object, PortableServer::POA>
	},
	{ // epv
		S::_activate_object
	}
};

}
}

#endif
