#include <CORBA/LifeCycleRefCntLink.h>
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

}
}
