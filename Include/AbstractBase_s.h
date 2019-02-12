#ifndef NIRVANA_ORB_ABSTRACTBASE_S_H_
#define NIRVANA_ORB_ABSTRACTBASE_S_H_

#include "AbstractBase_c.h"
#include "Interface_s.h"

// Servant part of an interface

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, AbstractBase>
{
public:
	static const typename Bridge <AbstractBase>::EPV epv_;

protected:
	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		Bridge <Interface>* ret = nullptr;
		try {
			ret = S::_implementation (base)._find_interface (id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		if (ret)
			ret = (ret->_epv ().duplicate) (ret, env);
		return ret;
	}

public:
	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		try {
			_check_pointer (itf, Skeleton <S, I>::epv_.interface);
			return S::_duplicate (static_cast <Bridge <I>*> (itf));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		try {
			_check_pointer (itf, Skeleton <S, I>::epv_.interface);
			S::_release (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

template <class S>
const Bridge <AbstractBase>::EPV Skeleton <S, AbstractBase>::epv_ = {
	{	// interface
		&(S::template __duplicate <AbstractBase>),
		&(S::template __release <AbstractBase>)
	},
	{	// epv
		&S::__find_interface
	}
};

}
}

#endif
