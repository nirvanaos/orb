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
	void clear ()
	{
		exception_free ();
	}
};

typedef Environment* Environment_ptr;
typedef Nirvana::LocalImpl <Environment>::_var_type Environment_var;
typedef Nirvana::LocalImpl <Environment>::_inout_type Environment_inout;
typedef Nirvana::LocalImpl <Environment>::_out_type Environment_out;

}

#endif

