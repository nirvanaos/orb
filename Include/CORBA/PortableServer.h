#ifndef NIRVANA_ORB_PORTABLESERVER_H_
#define NIRVANA_ORB_PORTABLESERVER_H_

#include "ImplementationTied.h"
#include "ImplementationPOA.h"

namespace PortableServer {

typedef CORBA::Nirvana::ServantPOA <CORBA::Nirvana::ServantBase> ServantBase;
typedef ServantBase* Servant;
typedef Servant_var <ServantBase> ServantBase_var;

inline void release (Servant s)
{
	if (s)
		s->_remove_ref ();
}

}

#endif
