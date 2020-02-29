#include <CORBA/ReferenceCounterLink.h>
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

void* ReferenceCounterLink::operator new (size_t size)
{
	return g_object_factory->memory_allocate (size);
}

void ReferenceCounterLink::operator delete (void* p, size_t size)
{
	g_object_factory->memory_release (p, size);
}

ReferenceCounterLink::ReferenceCounterLink (const Bridge <DynamicServant>::EPV& epv) :
	Bridge <DynamicServant> (epv),
	reference_counter_ (g_object_factory->create_reference_counter (
		DynamicServant_ptr (static_cast <DynamicServant*> (static_cast <Bridge <DynamicServant>*> (this)))))
{}

}
}
