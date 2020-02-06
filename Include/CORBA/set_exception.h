/// Static set exception to environment.
#ifndef NIRVANA_ORB_SET_EXCEPTION_H_
#define NIRVANA_ORB_SET_EXCEPTION_H_

#include "Bridge.h"
#include "system_exceptions.h"

#define DECLARE_FUNCTION(e) void set_##e (EnvironmentBridge* env);

namespace CORBA {
namespace Nirvana {

SYSTEM_EXCEPTIONS (DECLARE_FUNCTION)

}
}

#undef DECLARE_FUNCTION

#endif
