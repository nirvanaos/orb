#ifndef NIRVANA_ORB_LIFECYCLE_H_
#define NIRVANA_ORB_LIFECYCLE_H_

#include "Bridge.h"
#include "set_exception.h"
#include "Exception.h"

namespace CORBA {
namespace Nirvana {

class LifeCycleStatic
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, EnvironmentBridge*)
	{
		return itf;
	}

	template <class I>
	static void __release (Interface*)
	{}
};

//! Dynamic servant life cycle.
//! \tparam S Class implementing `_duplicate()' and `_release()' operations.
template <class S>
class LifeCycleDynamic
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, EnvironmentBridge* env)
	{
		try {
			return S::_duplicate_impl (static_cast <Bridge <I>*> (itf));
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
			S::_release_impl (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

//! Life cycle with reference counting.
template <class S>
class LifeCycleRefCnt :
	public LifeCycleDynamic <S>
{
public:
	template <class I>
	static Bridge <I>* _duplicate_impl (Bridge <I>* itf)
	{
		if (itf)
			S::_implementation (itf)._add_ref ();
		return itf;
	}

	template <class I>
	static void _release_impl (Bridge <I>* itf)
	{
		if (itf)
			S::_implementation (itf)._remove_ref ();
	}
};

//! Non copyable reference.
template <class S>
class LifeCycleNoCopy
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, EnvironmentBridge* env)
	{
		set_NO_IMPLEMENT (env);
		return nullptr;
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
