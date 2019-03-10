// Nirvana project
// Object Request Broker
// PortableServer::POA skeleton
#ifndef NIRVANA_ORB_POA_S_H_
#define NIRVANA_ORB_POA_S_H_

#include "POA_c.h"
#include "Object_s.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::PortableServer::POA>
{
public:
	static const typename Bridge < ::PortableServer::POA>::EPV epv_;

	static Bridge <Interface>* _find_interface (S& servant, const Char* id)
	{
		return Interface::_duplicate (InterfaceFinder <S, ::PortableServer::POA, Object>::find (servant, id));
	}

protected:
	static const Char* _activate_object (Bridge < ::PortableServer::POA>* obj, ClientBridge <ServantLinks>* servant, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).activate_object (servant);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge < ::PortableServer::POA>::EPV Skeleton <S, ::PortableServer::POA>::epv_ = {
	{ // interface
		Bridge < ::PortableServer::POA>::interface_id_,
		S::template __duplicate <Object>,
		S::template __release <Object>
	},
	{ // base
		S::template _wide <Object, ::PortableServer::POA>
	},
	{ // epv
		S::_activate_object
	}
};

}
}

#endif
