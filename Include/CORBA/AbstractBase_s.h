#ifndef NIRVANA_ORB_ABSTRACTBASE_S_H_
#define NIRVANA_ORB_ABSTRACTBASE_S_H_

#include "AbstractBase.h"

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
	static Interface* __query_interface (Bridge <AbstractBase>* base, ABI_in <String> id, Interface* env)
	{
		Interface* ret = 0;
		try {
			ret = TypeI <Interface>::VT_ret (S::_implementation (base)._query_interface (Type <String>::in (id)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return ret;
	}
};

template <class S>
const Bridge <AbstractBase>::EPV Skeleton <S, AbstractBase>::epv_ = {
	{	// header
		Bridge <AbstractBase>::repository_id_,
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
