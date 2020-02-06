#ifndef NIRVANA_ORB_ENVIRONMENT_S_H_
#define NIRVANA_ORB_ENVIRONMENT_S_H_

#include "Environment_c.h"

namespace CORBA {
namespace Nirvana {

template <class S, class I> class Skeleton;

template <class S>
class Skeleton <S, ::CORBA::Environment>
{
public:
	static const typename Bridge < ::CORBA::Environment>::EPV epv_;

protected:
	static void _exception_set (Bridge < ::CORBA::Environment>* obj, Long code, const char* rep_id, const void* param)
	{
		try {
			S::_implementation (obj).exception_set (code, rep_id, param);
		} catch (...) {
		}
	}

	static const Char* _exception_id (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			return S::_implementation (obj).exception_id ();
		} catch (...) {
		}
		return nullptr;
	}

	static const void* _exception_value (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			return S::_implementation (obj).exception_value ();
		} catch (...) {
		}
		return nullptr;
	}

	static void _exception_free (Bridge < ::CORBA::Environment>* obj)
	{
		try {
			S::_implementation (obj).exception_free ();
		} catch (...) {
		}
	}
};

template <class S>
const Bridge < ::CORBA::Environment>::EPV Skeleton <S, ::CORBA::Environment>::epv_ = {
	{ // interface
		Bridge < ::CORBA::Environment>::interface_id_,
		S::template __duplicate < ::CORBA::Environment>,
		S::template __release < ::CORBA::Environment>
	},
	{ // epv
		S::_exception_set,
		S::_exception_id,
		S::_exception_value,
		S::_exception_free
	}
};

}
}

#endif
