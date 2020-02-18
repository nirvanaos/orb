#ifndef NIRVANA_ORB_CORBA_ENVIRONMENT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENT_H_

#include "EnvironmentImpl.h"
#include "DynamicLocal.h"

namespace CORBA {

typedef Environment* Environment_ptr;
typedef PortableServer::Servant_var <Environment> Environment_var;
typedef Environment_var& Environment_inout;
typedef PortableServer::Servant_out <Environment> Environment_out;

typedef Environment* Environment_ptr;

//! CORBA::Environment
class Environment :
	public Nirvana::EnvironmentImpl <Environment>,
	public Nirvana::DynamicLocal <Environment>
{
public:
	void clear ()
	{
		exception_free ();
	}
};

}

#endif

