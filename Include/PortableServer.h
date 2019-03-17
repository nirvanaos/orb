#ifndef NIRVANA_ORB_PORTABLESERVER_H_
#define NIRVANA_ORB_PORTABLESERVER_H_

#include "ImplementationPOA.h"

namespace PortableServer {

typedef ::CORBA::Nirvana::ServantPOA < ::CORBA::Nirvana::ServantBase> ServantBase;
typedef ::CORBA::Nirvana::ServantBase_ptr Servant;
typedef ::CORBA::Nirvana::ServantBase_var ServantBase_var;
typedef ::CORBA::Nirvana::ServantBase_out ServantBase_out;

typedef ::CORBA::Nirvana::ServantPOA <POA> POA_PortableServer_POA;

}

#endif
