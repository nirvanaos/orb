#include <CORBA/ServantBaseLink.h>
#include <CORBA/POA.h>
#include <CORBA/core_objects.h>

namespace CORBA {
namespace Nirvana {

void ServantBaseLink::_construct ()
{
	core_object_ = g_object_factory->create_servant (PortableServer::Servant (this));
}

}
}