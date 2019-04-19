#ifndef NIRVANA_ORB_ENVIRONMENTIMPL_H_
#define NIRVANA_ORB_ENVIRONMENTIMPL_H_

#include "ServantStatic.h"
#include "Environment.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ::CORBA::Environment>
{
public:
	static const typename Bridge <Environment>::EPV epv_;

protected:
	static void _set_exception (Bridge <Environment>* obj, Long code, const char* rep_id, const void* param)
	{
		try {
			S::_implementation (obj).set_exception (code, rep_id, param);
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
		S::_set_exception
	}
};

class Environment :
	public EnvironmentBase,
	public InterfaceImpl <Environment, ::CORBA::Environment>,
	public ServantTraits <Environment>,
	public LifeCycleStatic <>
{
public:
	void set_exception (Long code, const char* rep_id, const void* param)
	{
		EnvironmentBase::set_exception (code, rep_id, param);
	}
};

class EnvironmentEx :
	public EnvironmentBase,
	public InterfaceImpl <EnvironmentEx, ::CORBA::Environment>,
	public ServantTraits <EnvironmentEx>,
	public LifeCycleStatic <>
{
public:
	EnvironmentEx (const ExceptionEntry* const* user_exceptions) :
		user_exceptions_ (user_exceptions)
	{}

	void set_exception (Long code, const char* rep_id, const void* param)
	{
		EnvironmentBase::set_exception (code, rep_id, param, user_exceptions_);
	}

private:
	const ExceptionEntry* const* user_exceptions_;
};

}
}

#endif
