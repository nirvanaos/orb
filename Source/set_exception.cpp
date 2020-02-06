#include <CORBA/set_exception.h>
#include <CORBA/Exception.h>
#include <CORBA/Environment_c.h>

namespace CORBA {
namespace Nirvana {

void set_NO_IMPLEMENT (EnvironmentBridge* env)
{
	env->set_exception (NO_IMPLEMENT ());
}

void set_BAD_TYPECODE (EnvironmentBridge* env)
{
	env->set_exception (BAD_TYPECODE ());
}

}
}
