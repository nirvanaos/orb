#ifndef NIRVANA_ORB_CORBA_ENVIRONMENT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENT_H_

#include "EnvironmentImpl.h"
#include "LocalImpl.h"

namespace CORBA {

//! CORBA::Environment
class Environment :
	public Nirvana::EnvironmentImpl <Environment>,
	public Nirvana::LocalImpl <Environment>
{
public:
	Environment () {}

	Environment (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment (Nirvana::EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment& operator = (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}

	Environment& operator = (Nirvana::EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}
};

typedef Environment* Environment_ptr;
typedef Nirvana::LocalImpl <Environment>::_var_type Environment_var;
typedef Nirvana::LocalImpl <Environment>::_inout_type Environment_inout;
typedef Nirvana::LocalImpl <Environment>::_out_type Environment_out;

}

#endif

