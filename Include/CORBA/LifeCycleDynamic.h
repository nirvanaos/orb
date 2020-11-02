#ifndef NIRVANA_ORB_LIFECYCLEDYNAMIC_H_
#define NIRVANA_ORB_LIFECYCLEDYNAMIC_H_

#include "Exception.h"

namespace CORBA {
namespace Nirvana {

//! Dynamic servant life cycle.
//! \tparam S Class implementing `_duplicate()' and `_release()' operations.
template <class S>
class LifeCycleDynamic
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, Interface* env)
	{
		try {
			return S::_duplicate (static_cast <Bridge <I>*> (itf));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}

	template <class I>
	static void __release (Interface* itf)
	{
		try {
			S::_release (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

}
}

#endif
