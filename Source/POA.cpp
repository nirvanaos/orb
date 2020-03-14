#include <CORBA/POA.h>
#include <Nirvana/OLF.h>

namespace PortableServer {

DEFINE_CORBA_INTERFACE_EXCEPTION (PortableServer, POA, ServantAlreadyActive);
DEFINE_CORBA_INTERFACE_EXCEPTION (PortableServer, POA, ObjectNotActive);

}
