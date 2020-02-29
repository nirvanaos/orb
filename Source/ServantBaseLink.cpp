#include <CORBA/ServantBaseLink.h>
#include <CORBA/POA.h>
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

void ServantBaseLink::_construct ()
{
	servant_base_ = g_object_factory->create_servant (PortableServer::Servant (this));
}

Interface* ServantBaseLink::_get_proxy ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = _default_POA ();
		String objid = poa->activate_object (servant_base_);
	}
	Interface* proxy = AbstractBase_ptr (servant_base_)->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

}
}