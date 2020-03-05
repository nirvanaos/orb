#include <CORBA/POA.h>
#include <CORBA/TypeCodeException.h>

namespace PortableServer {

DEFINE_INTERFACE_EXCEPTION (POA, ServantAlreadyActive, PORTABLESERVER_REPOSITORY_ID ("POA/ServantAlreadyActive"));
DEFINE_INTERFACE_EXCEPTION (POA, ObjectNotActive, PORTABLESERVER_REPOSITORY_ID ("POA/ObjectNotActive"));

}
