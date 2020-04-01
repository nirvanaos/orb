#ifndef NIRVANA_ORB_IOREQUEST_S_H_
#define NIRVANA_ORB_IOREQUEST_S_H_

#include "IORequest.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, IORequest>
{
public:
	static const typename Bridge <IORequest>::EPV epv_;

protected:
	static Interface* _get_marshaler (Bridge <IORequest>* _b, EnvironmentBridge* _env)
	{
		try {
			return TypeI <Marshal>::VT_ret (S::_implementation (_b).marshaler ());
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return nullptr;
	}

	static void _exception (Bridge <IORequest>* _b, Type <Any>::ABI_inout exc)
	{
		try {
			S::_implementation (_b).exception (Type <Any>::inout (exc));
		} catch (...) {
			// Swallow exception. TODO: Log.
		}
	}

	static void _success (Bridge <IORequest>* _b, EnvironmentBridge* _env)
	{
		try {
			S::_implementation (_b).success ();
		} catch (const Exception & e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

template <class S>
const Bridge <IORequest>::EPV Skeleton <S, IORequest>::epv_ = {
	{ // header
		Bridge <IORequest>::repository_id_,
		S::template __duplicate <IORequest>,
		S::template __release <IORequest>
	},
	{ // epv
		S::_get_marshaler,
		S::_exception,
		S::_success
	}
};

}
}

#endif
