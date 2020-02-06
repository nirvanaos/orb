#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_

#include "EnvironmentImpl.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class EnvironmentImpl :
	public EnvironmentBase,
	public Skeleton <S, ::CORBA::Environment>,
	public ServantTraits <S>
{
protected:
	EnvironmentImpl () :
		EnvironmentBase (Skeleton <S, ::CORBA::Environment>::epv_)
	{}
};

class Environment :
	public EnvironmentImpl <Environment>,
	public LifeCycleNoCopy <Environment>
{};

template <class ... Exceptions>
class EnvironmentEx :
	public EnvironmentImpl <EnvironmentEx <Exceptions...> >,
	public LifeCycleNoCopy <Environment>
{
public:
	void exception_set (Long code, const char* rep_id, const void* param)
	{
		EnvironmentBase::exception_set (code, rep_id, param, user_exceptions_);
	}

private:
	static const ExceptionEntry user_exceptions_ [];
};

template <class ... Exceptions>
const ExceptionEntry EnvironmentEx <Exceptions...>::user_exceptions_ [] = {
	{ Exceptions::repository_id_, Exceptions::_create }...,
	{0}
};

}
}

#endif
