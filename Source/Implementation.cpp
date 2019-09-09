#include <CORBA/Implementation.h>

namespace CORBA {
namespace Nirvana {

ReferenceCounterLink::ReferenceCounterLink (DynamicServant_ptr dynamic) :
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

void ServantBaseLink::_implicitly_activate ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = servant ()->_default_POA ();
		poa->activate_object (servant_base_);
	}
}

ReferenceCounter_ptr LocalObjectLink::_construct (Bridge <AbstractBase>* base, Bridge <DynamicServant>* dynamic)
{
	LocalObject_ptr obj = ObjectFactory::singleton ()->create_local_object (base, dynamic);
	object_ = obj;
	return obj;
}

}
}