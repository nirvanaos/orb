#include <CORBA/Implementation.h>
#include <CORBA/ObjectFactory.h>
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

ReferenceCounterLink::ReferenceCounterLink (Bridge <DynamicServant>* dynamic) :
	reference_counter_ (g_object_factory->create_reference_counter (
		DynamicServant_ptr (static_cast <DynamicServant*> (dynamic))))
{}

ReferenceCounterLink::~ReferenceCounterLink ()
{
	release (reference_counter_);
}

void ServantBaseLink::_construct (Bridge <DynamicServant>* dynamic)
{
	servant_base_ = g_object_factory->create_servant (
		I_ptr <PortableServer::ServantBase> (&static_cast <PortableServer::ServantBase&> (static_cast <Bridge <PortableServer::ServantBase>&> (*this))),
		DynamicServant_ptr (static_cast <DynamicServant*> (dynamic)));
}

Interface* ServantBaseLink::_get_proxy ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = servant ()->_default_POA ();
		poa->activate_object (servant_base_);
	}
	Interface* proxy = AbstractBase_ptr (servant_base_)->_query_interface (nullptr);
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
	Interface* proxy = AbstractBase_ptr (object_)->_query_interface (nullptr);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

}
}