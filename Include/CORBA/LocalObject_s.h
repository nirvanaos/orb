#ifndef NIRVANA_ORB_LOCALOBJECT_S_H_
#define NIRVANA_ORB_LOCALOBJECT_S_H_

#include "LocalObject.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, LocalObject>
{
public:
	static const typename Bridge <LocalObject>::EPV epv_;

protected:
	static ABI_boolean __non_existent (Bridge <LocalObject>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <LocalObject>::EPV Skeleton <S, LocalObject>::epv_ = {
	{ // interface
		Bridge <LocalObject>::repository_id_,
		S::template __duplicate <LocalObject>,
		S::template __release <LocalObject>
	},
	{ // base
		S::template _wide_object <LocalObject>,
		S::template _wide <ReferenceCounter, LocalObject>
	},
	{ // epv
		S::__non_existent
	}
};

}
}

#endif
