#ifndef NIRVANA_ORB_DYNAMICSERVANT_S_H_
#define NIRVANA_ORB_DYNAMICSERVANT_S_H_

#include "DynamicServant.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, DynamicServant>
{
public:
	static const typename Bridge <DynamicServant>::EPV epv_;

	void _delete ()
	{
		delete &static_cast <S&> (*this);
	}

protected:
	static void __delete (Bridge <DynamicServant>* obj, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj)._delete ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}
};

template <class S>
const Bridge <DynamicServant>::EPV Skeleton <S, DynamicServant>::epv_ = {
	{ // interface
		Bridge <DynamicServant>::interface_id_,
		S::template __duplicate <DynamicServant>,
		S::template __release <DynamicServant>
	},
	{ // epv
		S::__delete
	}
};

}
}

#endif
