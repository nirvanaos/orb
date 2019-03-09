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
	typedef S ServantType;
	static const typename Bridge <AbstractBase>::EPV epv_;

protected:
	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		Bridge <Interface>* ret = nullptr;
		try {
			ret = S::_servant (base)._find_interface (id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return ret;
	}

public:
	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env)
	{
		try {
			if (!RepositoryId::compatible (Bridge <Base>::interface_id_, id))
				throw MARSHAL ();
			return &static_cast <Bridge <Base>&> (S::_servant (derived));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}
};

template <class S>
const Bridge <AbstractBase>::EPV Skeleton <S, AbstractBase>::epv_ = {
	{	// interface
		Bridge <AbstractBase>::interface_id_,
		&S::template __duplicate <AbstractBase>,
		&S::template __release <AbstractBase>
	},
	{	// epv
		&S::__find_interface
	}
};

}
}

#endif
