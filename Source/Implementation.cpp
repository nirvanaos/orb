#include <CORBA/Implementation.h>
#include <CORBA/ObjectFactory.h>
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

void LocalObjectLink::_construct (Bridge <Object>* impl)
{
	object_ = g_object_factory->create_local_object (Object_ptr (static_cast <Object*> (impl)));
}

Interface* LocalObjectLink::_get_proxy ()
{
	Interface* proxy = AbstractBase_ptr (object_)->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

}
}