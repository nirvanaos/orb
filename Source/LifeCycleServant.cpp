#include <CORBA/LifeCycleServant.h>
#include <CORBA/core_objects.h>

namespace CORBA {
namespace Nirvana {

ReferenceCounterLink::ReferenceCounterLink (const Bridge <DynamicServant>::EPV& epv) :
	Bridge <DynamicServant> (epv),
	reference_counter_ (g_object_factory->create_reference_counter (
		DynamicServant_ptr (static_cast <DynamicServant*> (static_cast <Bridge <DynamicServant>*> (this)))))
{}

}
}
