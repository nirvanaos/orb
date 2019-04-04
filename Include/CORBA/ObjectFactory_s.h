#ifndef NIRVANA_ORB_OBJECTFACTORY_S_H_
#define NIRVANA_ORB_OBJECTFACTORY_S_H_

#include "ObjectFactory_c.h"
#include "Interface_s.h"

// ObjectFactory skeleton

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ObjectFactory>
{
public:
	static const typename Bridge <ObjectFactory>::EPV epv_;

protected:
	static BridgeMarshal <PortableServer::ServantBase>* _create_servant (Bridge <ObjectFactory>* obj, BridgeMarshal <PortableServer::ServantBase>* servant, BridgeMarshal <DynamicServant>* dynamic, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_servant (servant, dynamic);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static BridgeMarshal <LocalObject>* _create_local_object (Bridge <ObjectFactory>* obj, BridgeMarshal <AbstractBase>* base, BridgeMarshal <DynamicServant>* dynamic, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_local_object (base, dynamic);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static BridgeMarshal <ReferenceCounter>* _create_reference_counter (Bridge <ObjectFactory>* obj, BridgeMarshal <DynamicServant>* dynamic, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_reference_counter (dynamic);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <ObjectFactory>::EPV Skeleton <S, ObjectFactory>::epv_ = {
	{ // interface
		Bridge <ObjectFactory>::interface_id_,
		S::template __duplicate <ObjectFactory>,
		S::template __release <ObjectFactory>
	},
	{ // base
		S::template _wide <AbstractBase, ObjectFactory>
	},
	{ // epv
		S::_create_servant,
		S::_create_local_object,
		S::_create_reference_counter
	}
};

}
}

#endif
