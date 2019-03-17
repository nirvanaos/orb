#include <Implementation.h>

namespace CORBA {
namespace Nirvana {

ServantBaseLink::ServantBaseLink (const Bridge <ServantBase>::EPV& servant_base, Bridge <DynamicServant>& dynamic_servant) :
	Bridge <ServantBase> (servant_base),
	servant_base_ (g_object_adapter->create_servant (this, &dynamic_servant))
{}

void ServantBaseLink::_implicitly_activate ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = ServantBase_ptr (this)->_default_POA ();
		poa->activate_object (servant_base_);
	}
}

LocalObjectLink::LocalObjectLink (const EPV& epv) :
	Bridge <DynamicServant> (epv),
	object_ (g_object_adapter->create_local_object (this))
{}

}
}