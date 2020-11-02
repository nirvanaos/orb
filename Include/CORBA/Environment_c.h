#ifndef NIRVANA_ORB_ENVIRONMENT_C_H_
#define NIRVANA_ORB_ENVIRONMENT_C_H_

#include "Bridge.h"

namespace CORBA {

class Environment;

namespace Nirvana {

BRIDGE_BEGIN (::CORBA::Environment, CORBA_REPOSITORY_ID ("Environment"))
void (*exception_set) (Bridge < ::CORBA::Environment>*, Short code, const Char* rep_id, const void* param);
const Char* (*exception_id) (Bridge < ::CORBA::Environment>*);
const void* (*exception_value) (Bridge < ::CORBA::Environment>*);
void (*exception_free) (Bridge < ::CORBA::Environment>*);
BRIDGE_END ()

}
}

#endif
