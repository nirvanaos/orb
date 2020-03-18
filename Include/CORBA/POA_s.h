// Nirvana project
// Object Request Broker
// PortableServer::POA skeleton
#ifndef NIRVANA_ORB_POA_S_H_
#define NIRVANA_ORB_POA_S_H_

#include "POA.h"
#include "Object_s.h"
#include "ImplementationLocal.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, PortableServer::POA>
{
public:
	static const typename Bridge <PortableServer::POA>::EPV epv_;

protected:
	static Type <String>::ABI_ret _activate_object (Bridge <PortableServer::POA>* obj, Interface* servant, EnvironmentBridge* env)
	{
		try {
			return Type <String>::ret (S::_implementation (obj).activate_object (TypeI <PortableServer::ServantBase>::in (servant)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <String>::ABI_ret ();
	}

	static void _deactivate_object (Bridge <PortableServer::POA>* obj, Type <String>::ABI_in objid, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj).deactivate_object (Type <String>::in (objid));
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}
};

template <class S>
const Bridge <PortableServer::POA>::EPV Skeleton <S, PortableServer::POA>::epv_ = {
	{ // header
		Bridge <PortableServer::POA>::repository_id_,
		S::template __duplicate <PortableServer::POA>,
		S::template __release <PortableServer::POA>
	},
	{ // base
		S::template _wide_object <PortableServer::POA>
	},
	{ // epv
		S::_activate_object,
		S::_deactivate_object
	}
};

template <class S>
class Servant <S, PortableServer::POA> :
	public ImplementationLocal <S, PortableServer::POA>
{};

}
}

#endif
