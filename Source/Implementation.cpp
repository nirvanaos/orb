#include <CORBA/Implementation.h>
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

void ServantBaseLink::_construct ()
{
	servant_base_ = g_object_factory->create_servant (ServantBase_ptr (this));
}

Interface* ServantBaseLink::_get_proxy ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = servant ()->_default_POA ();
		poa->activate_object (servant_base_);
	}
	Interface* proxy = AbstractBase_ptr (servant_base_)->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

void LocalObjectLink::_construct (Bridge <AbstractBase>* base)
{
	object_ = g_object_factory->create_local_object (
		AbstractBase_ptr (static_cast <AbstractBase*> (base)));
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