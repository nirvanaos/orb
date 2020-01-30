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

class Environment :
	public EnvironmentBase,
	public InterfaceImpl <Environment, ::CORBA::Environment>,
	public ServantTraits <Environment>,
	public LifeCycleStatic
{};

template <class ... Exceptions>
class EnvironmentEx :
	public EnvironmentBase,
	public InterfaceImpl <EnvironmentEx <Exceptions...>, ::CORBA::Environment>,
	public ServantTraits <EnvironmentEx <Exceptions...>>,
	public LifeCycleStatic
{
public:
	EnvironmentEx (const ExceptionEntry** user_exceptions) :
		user_exceptions_ (user_exceptions)
	{}

	void exception_set (Long code, const char* rep_id, const void* param)
	{
		EnvironmentBase::exception_set (code, rep_id, param, &user_exceptions_);
	}

private:
	static const ExceptionEntry user_exceptions_ [];
};

template <class ... Exceptions>
const ExceptionEntry EnvironmentEx <Exceptions...>::user_exceptions_[] = {
	{ Exceptions::repository_id_, Exceptions::_create }...,
	{0}
};

}
}

#endif
