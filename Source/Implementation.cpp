#include <CORBA/Implementation.h>
#include <CORBA/ObjectFactory.h>
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

void ServantBaseLink::_construct (Bridge <DynamicServant>* dynamic)
{
	servant_base_ = g_object_factory->create_servant (PortableServer::Servant (this),
		DynamicServant_ptr (static_cast <DynamicServant*> (dynamic)));
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

ReferenceCounter_ptr LocalObjectLink::_construct (Bridge <AbstractBase>* base, Bridge <DynamicServant>* dynamic)
{
	LocalObject_ptr obj = g_object_factory->create_local_object (
		AbstractBase_ptr (static_cast <AbstractBase*> (base)), 
		DynamicServant_ptr (static_cast <DynamicServant*> (dynamic)));
	object_ = obj;
	return obj;
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