#ifndef NIRVANA_ORB_LIFECYCLENOCOPY_H_
#define NIRVANA_ORB_LIFECYCLENOCOPY_H_

#include "set_exception.h"

namespace CORBA {
namespace Nirvana {

//! Non copyable reference.
template <class S>
class LifeCycleNoCopy
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, Interface* env)
	{
		set_NO_IMPLEMENT (env);
		return 0;
	}

	template <class I>
	static void __release (Interface* itf)
	{
		try {
			delete& S::_implementation (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

}
}

#endif
