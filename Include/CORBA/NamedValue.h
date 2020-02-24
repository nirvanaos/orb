#ifndef NIRVANA_ORB_NAMEDVALUE_H_
#define NIRVANA_ORB_NAMEDVALUE_H_

#include "Any.h"

namespace CORBA {

class NamedValue;
typedef Nirvana::I_ptr <NamedValue> NamedValue_ptr;
typedef Nirvana::I_var <NamedValue> NamedValue_var;
typedef Nirvana::I_out <NamedValue> NamedValue_out;
typedef Nirvana::I_inout <NamedValue> NamedValue_inout;

namespace Nirvana {

BRIDGE_BEGIN (NamedValue, CORBA_REPOSITORY_ID (NamedValue))
const Char* (*name) (Bridge <NamedValue>*, EnvironmentBridge*);
const Any& (*value) (Bridge <NamedValue>*, EnvironmentBridge*);
Flags (*flags) (Bridge <NamedValue>*, EnvironmentBridge*);
BRIDGE_END ()

}

}

#endif
