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
	static Bridge <Interface>* __query_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		Bridge <Interface>* ret = nullptr;
		try {
			ret = S::_implementation (base)._query_interface (id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return ret;
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
		&S::__query_interface
	}
};

}
}

#endif
