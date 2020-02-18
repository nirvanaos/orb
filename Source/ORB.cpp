#include <CORBA/ORB.h>

namespace CORBA {

void ORB::create_environment (Environment_out env)
{
	env = new Environment ();
}

}