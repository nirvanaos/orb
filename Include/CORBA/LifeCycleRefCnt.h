#ifndef NIRVANA_ORB_LIFECYCLEREFCNT_H_
#define NIRVANA_ORB_LIFECYCLEREFCNT_H_

#include "LifeCycleDynamic.h"

namespace CORBA {
namespace Nirvana {

//! Life cycle with reference counting.
template <class S>
class LifeCycleRefCnt :
	public LifeCycleDynamic <S>
{
public:
	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		if (itf)
			S::_implementation (itf)._add_ref ();
		return itf;
	}

	template <class I>
	static void _release (Bridge <I>* itf)
	{
		if (itf)
			S::_implementation (itf)._remove_ref ();
	}
};

}
}

#endif
