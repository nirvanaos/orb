#ifndef NIRVANA_ORB_OBJECTADAPTER_S_H_
#define NIRVANA_ORB_OBJECTADAPTER_S_H_

#include "ObjectAdapter_c.h"
#include "Interface_s.h"

namespace CORBA {
namespace Nirvana {

// ObjectAdapter skeleton
template <class S>
class Skeleton <S, ObjectAdapter>
{
public:
	static const typename Bridge <ObjectAdapter>::EPV epv_;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <ObjectAdapter>::_primary_interface (), id))
			return &static_cast <Bridge <ObjectAdapter>&> (base);
		else
			return nullptr;
	}

protected:
	static ServantLinks* _create_servant (Bridge <ObjectAdapter>* obj, Bridge <ServantBase>* servant, const Char* type_id, EnvironmentBridge* env)
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

	static void _destroy_servant (Bridge <ObjectAdapter>* obj, ServantLinks* servant, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj).destroy_servant (servant);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}

	static void _activate_object (Bridge <ObjectAdapter>* obj, ServantLinks* servant, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj).activate_object (servant);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}
};

template <class S>
const Bridge <ObjectAdapter>::EPV Skeleton <S, ObjectAdapter>::epv_ = {
	{ // interface
		S::template _duplicate <ObjectAdapter>,
		S::template _release <ObjectAdapter>
	},
	{ // base
		S::template _wide <AbstractBase, ObjectAdapter>
	},
	{ // epv
		S::_create_servant,
		S::_destroy_servant,
		S::_activate_object
	}
};

}
}

#endif
