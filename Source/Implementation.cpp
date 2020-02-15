#include <CORBA/Implementation.h>
#include <CORBA/ObjectFactory.h>
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {

extern const ::Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory;

ReferenceCounterLink::ReferenceCounterLink (Bridge <DynamicServant>* dynamic) :
	reference_counter_ (g_object_factory->create_reference_counter (dynamic))
{}

ReferenceCounterLink::~ReferenceCounterLink ()
{
	release (reference_counter_);
}

void ServantBaseLink::_construct (Bridge <DynamicServant>* dynamic)
{
	servant_base_ = g_object_factory->create_servant (this, dynamic);
}

Bridge <Interface>* ServantBaseLink::_get_proxy ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = servant ()->_default_POA ();
		poa->activate_object (servant_base_);
	}
	Bridge <Interface>* proxy = AbstractBase_ptr (servant_base_)->_query_interface (nullptr);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return Interface::__duplicate (proxy);
}

ReferenceCounter_ptr LocalObjectLink::_construct (Bridge <AbstractBase>* base, Bridge <DynamicServant>* dynamic)
{
	LocalObject_ptr obj = g_object_factory->create_local_object (base, dynamic);
	object_ = obj;
	return obj;
}

Bridge <Interface>* LocalObjectLink::_get_proxy ()
{
	Bridge <Interface>* proxy = AbstractBase_ptr (object_)->_query_interface (nullptr);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return Interface::__duplicate (proxy);
}

}
}