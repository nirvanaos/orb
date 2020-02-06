/// Static set exception to environment.
/// TODO: Add more functions.
#ifndef NIRVANA_ORB_SET_EXCEPTION_H_
#define NIRVANA_ORB_SET_EXCEPTION_H_

#include "Bridge.h"

namespace CORBA {
namespace Nirvana {

void set_NO_IMPLEMENT (EnvironmentBridge* env);
void set_BAD_TYPECODE (EnvironmentBridge* env);

}
}

#endif
