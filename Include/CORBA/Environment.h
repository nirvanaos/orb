#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_

#include "EnvironmentImpl.h"

namespace CORBA {
namespace Nirvana {

class Environment :
	public EnvironmentImpl <Environment>,
	public LifeCycleNoCopy <Environment>
{
public:
	Environment () {}
	
	Environment (const Environment& src)
	{
		EnvironmentBase::operator = (src);
	}

	Environment (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment (const EnvironmentBase& src)
	{
		EnvironmentBase::operator = (src);
	}

	Environment (EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment& operator = (const EnvironmentBase& src);
	Environment& operator = (EnvironmentBase&& src) NIRVANA_NOEXCEPT;
};

}
}

#endif
