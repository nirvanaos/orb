#ifndef NIRVANA_ORB_ENVIRONMENTEX_H_
#define NIRVANA_ORB_ENVIRONMENTEX_H_

#include "Environment.h"
#include "TypeCodeException.h"

namespace CORBA {
namespace Nirvana {

template <class ... Exceptions>
class EnvironmentEx :
	public EnvironmentImpl <EnvironmentEx <Exceptions...> >,
	public LifeCycleNoCopy <EnvironmentEx <Exceptions...> >
{
public:
	EnvironmentEx () {}

	void exception_set (Long code, const char* rep_id, const void* param)
	{
		EnvironmentBase::exception_set (code, rep_id, param, user_exceptions_);
	}

private:
	static const ExceptionEntry user_exceptions_[];
};

template <class ... Exceptions>
const ExceptionEntry EnvironmentEx <Exceptions...>::user_exceptions_[] = {
	{ STATIC_BRIDGE (TypeCodeException <Exceptions>, TypeCode) }...,
	{0}
};

}
}

#endif
