#ifndef NIRVANA_ORB_DYNAMICSERVANT_S_H_
#define NIRVANA_ORB_DYNAMICSERVANT_S_H_

#include "DynamicServant_c.h"
#include "AbstractBase_s.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, DynamicServant>
{
public:
	static const typename Bridge <DynamicServant>::EPV epv_;

protected:
	static void __invoke (Bridge <DynamicServant>* obj, BridgeMarshal <Request>* request, EnvironmentBridge* env)
	{
		try {
			return S::_servant (obj)._invoke (request);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}

	static const Char* __primary_interface (Bridge <DynamicServant>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_servant (obj)._primary_interface ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	void _invoke (Request_ptr request)
	{
		throw NO_IMPLEMENT ();	// TODO: Implement.
	}
};

template <class S>
const Bridge <DynamicServant>::EPV Skeleton <S, DynamicServant>::epv_ = {
	{ // interface
		Bridge <DynamicServant>::interface_id_,
		S::template __duplicate <DynamicServant>,
		S::template __release <DynamicServant>
	},
	{ // base
		S::template _wide < ::CORBA::AbstractBase, DynamicServant>
	},
	{ // epv
		S::__invoke,
		S::__primary_interface
	}
};

}
}

#endif
