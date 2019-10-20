#include <CORBA/Implementation.h>

namespace CORBA {
namespace Nirvana {

ReferenceCounterLink::ReferenceCounterLink (Bridge <DynamicServant>* dynamic) :
	reference_counter_ (ObjectFactory::singleton ()->create_reference_counter (dynamic))
{}

ReferenceCounterLink::~ReferenceCounterLink ()
{
	release (reference_counter_);
}

void ServantBaseLink::_construct (Bridge <DynamicServant>* dynamic)
{
	servant_base_ = ObjectFactory::singleton ()->create_servant (this, dynamic);
}

Bridge <Interface>* ServantBaseLink::_implicitly_activate ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = servant ()->_default_POA ();
		poa->activate_object (servant_base_);
	}
	Bridge <Interface>* proxy = AbstractBase_ptr (servant_base_)->_query_interface (nullptr);
	if (!proxy)
		throw MARSHAL ();
	return Interface::__duplicate (proxy);
}

ReferenceCounter_ptr LocalObjectLink::_construct (Bridge <AbstractBase>* base, Bridge <DynamicServant>* dynamic)
{
	LocalObject_ptr obj = ObjectFactory::singleton ()->create_local_object (base, dynamic);
	object_ = obj;
	return obj;
}

Bridge <Interface>* LocalObjectLink::_implicitly_activate ()
{
	Bridge <Interface>* proxy = AbstractBase_ptr (object_)->_query_interface (nullptr);
	if (!proxy)
		throw MARSHAL ();
	return Interface::__duplicate (proxy);
}

}
}