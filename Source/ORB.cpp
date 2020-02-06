#include <CORBA/ORB.h>
#include <CORBA/CORBA_Environment.h>

namespace CORBA {

void ORB::create_environment (Environment_out env)
{
	env = Environment_ptr (new ::CORBA::Environment ());
}

}