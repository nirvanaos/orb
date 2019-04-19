#include <CORBA/ORB.h>
#include "CORBA_EnvironmentServant.h"

namespace CORBA {

void ORB::create_environment (Environment_out env)
{
	env = static_cast <::CORBA::Environment*> (new Nirvana::CORBA_EnvironmentServant ());
}

}