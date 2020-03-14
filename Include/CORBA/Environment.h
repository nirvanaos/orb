#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_

#include "EnvironmentImpl.h"

namespace CORBA {
namespace Nirvana {

class Environment :
	public EnvironmentImpl <Environment>
{
public:
	Environment () {}
	
	Environment (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment (EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment& operator = (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}

	Environment& operator = (EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}
};

}
}

#endif
