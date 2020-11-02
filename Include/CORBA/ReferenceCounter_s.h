#ifndef NIRVANA_ORB_REFERENCECOUNTER_S_H_
#define NIRVANA_ORB_REFERENCECOUNTER_S_H_

#include "ReferenceCounter.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ReferenceCounter>
{
public:
	static const typename Bridge <ReferenceCounter>::EPV epv_;

protected:
	static void __add_ref (Bridge <ReferenceCounter>* obj, Interface* env)
	{
		try {
			S::_implementation (obj)._add_ref ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static void __remove_ref (Bridge <ReferenceCounter>* obj, Interface* env)
	{
		try {
			S::_implementation (obj)._remove_ref ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
	}

	static ULong __refcount_value (Bridge <ReferenceCounter>* obj, Interface* env)
	{
		try {
			return S::_implementation (obj)._refcount_value ();
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return 0;
	}
};

template <class S>
const Bridge <ReferenceCounter>::EPV Skeleton <S, ReferenceCounter>::epv_ = {
	{ // header
		Bridge <ReferenceCounter>::repository_id_,
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
