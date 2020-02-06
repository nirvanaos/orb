#include <CORBA/set_exception.h>
#include <CORBA/exceptions.h>
#include <CORBA/Environment_c.h>

#define DEFINE_FUNCTION(e) void set_##e (EnvironmentBridge* env){ env->set_exception (SystemException::EC_##e, e::repository_id_, nullptr);}

namespace CORBA {
namespace Nirvana {

SYSTEM_EXCEPTIONS (DEFINE_FUNCTION)

}
}
