#include <CORBA/set_exception.h>
#include <CORBA/exceptions.h>
#include <CORBA/Environment_c.h>

#define DEF_EXCEPTION(e) void set_##e (EnvironmentBridge* env){ env->set_exception (SystemException::EC_##e, e::repository_id_, nullptr);}

namespace CORBA {
namespace Nirvana {

DEF_EXCEPTION(NO_IMPLEMENT)
DEF_EXCEPTION (BAD_TYPECODE)

}
}
