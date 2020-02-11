#ifndef NIRVANA_ORB_REFERENCECOUNTER_S_H_
#define NIRVANA_ORB_REFERENCECOUNTER_S_H_

#include "ReferenceCounter.h"
#include "Interface.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ReferenceCounter>
{
public:
	static const typename Bridge <ReferenceCounter>::EPV epv_;

protected:
	static void __add_ref (Bridge <ReferenceCounter>* obj, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj)._add_ref ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}

	static void __remove_ref (Bridge <ReferenceCounter>* obj, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj)._remove_ref ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}

	static ULong __refcount_value (Bridge <ReferenceCounter>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._refcount_value ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <ReferenceCounter>::EPV Skeleton <S, ReferenceCounter>::epv_ = {
	{ // interface
		Bridge <ReferenceCounter>::interface_id_,
		S::template __duplicate <ReferenceCounter>,
		S::template __release <ReferenceCounter>
	},
	{ // epv
		S::__add_ref,
		S::__remove_ref,
		S::__refcount_value
	}
};

}
}

#endif
