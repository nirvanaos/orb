#ifndef NIRVANA_ORB_OBJECTADAPTER_S_H_
#define NIRVANA_ORB_OBJECTADAPTER_S_H_

#include "ObjectAdapter_c.h"
#include "Interface_s.h"

// ObjectAdapter skeleton

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ObjectAdapter>
{
public:
	static const typename Bridge <ObjectAdapter>::EPV epv_;

protected:
	static BridgeMarshal <ServantLinks>* _create_servant (Bridge <ObjectAdapter>* obj, BridgeMarshal <ServantBase>* servant, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_servant (servant, type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static BridgeMarshal <Object>* _create_local_object (Bridge <ObjectAdapter>* obj, BridgeMarshal <AbstractBase>* base, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_local_object (base, type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <ObjectAdapter>::EPV Skeleton <S, ObjectAdapter>::epv_ = {
	{ // interface
		Bridge <ObjectAdapter>::interface_id_,
		S::template __duplicate <ObjectAdapter>,
		S::template __release <ObjectAdapter>
	},
	{ // base
		S::template _wide <AbstractBase, ObjectAdapter>
	},
	{ // epv
		S::_create_servant,
		S::_create_local_object
	}
};

}
}

#endif
