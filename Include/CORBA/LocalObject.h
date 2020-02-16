#ifndef NIRVANA_ORB_LOCALOBJECT_H_
#define NIRVANA_ORB_LOCALOBJECT_H_

#include "Object.h"
#include "ReferenceCounter.h"

namespace CORBA {

class LocalObject;
typedef Nirvana::I_ptr <LocalObject> LocalObject_ptr;
typedef Nirvana::I_var <LocalObject> LocalObject_var;
typedef Nirvana::I_out <LocalObject> LocalObject_out;
typedef Nirvana::I_inout <LocalObject> LocalObject_inout;

namespace Nirvana {

BRIDGE_BEGIN (LocalObject)
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BASE_STRUCT_ENTRY (ReferenceCounter, _ReferenceCounter)
BRIDGE_EPV
BRIDGE_END ()

}

class LocalObject : public Nirvana::ClientInterface <LocalObject, Object, Nirvana::ReferenceCounter>
{};

}

#endif
