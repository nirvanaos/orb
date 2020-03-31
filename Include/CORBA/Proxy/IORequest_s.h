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

	static void _system_exception (Bridge <IORequest>* _b, Long code, Type <String>::ABI_in rep_id, ULong minor, ABI_enum completed)
	{
		try {
			S::_implementation (_b).system_exception (code, Type <String>::in (rep_id), minor, Type <CompletionStatus>::in (completed));
		} catch (...) {
			// Swallow exception. TODO: Log.
		}
	}

	static void _user_exception (Bridge <IORequest>* _b, Interface* tc, ::Nirvana::ConstPointer data)
	{
		try {
			S::_implementation (_b).user_exception (TypeI <TypeCode>::in (tc), data);
		} catch (...) {
			// Swallow exception. TODO: Log.
		}
	}

	static void _unknown_exception (Bridge <IORequest>* _b)
	{
		try {
			S::_implementation (_b).unkown_exception ();
		} catch (...) {
			// Swallow exception. TODO: Log.
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
		S::_system_exception,
		S::_user_exception,
		S::_unknown_exception
	}
};

}
}

#endif
