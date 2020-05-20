#ifndef NIRVANA_ORB_SERVANTMEMORY_H_
#define NIRVANA_ORB_SERVANTMEMORY_H_

#include "core_objects.h"

namespace CORBA {
namespace Nirvana {

class ServantMemory
{
public:
	void* operator new (size_t size)
	{
		return g_object_factory->memory_allocate (size);
	}

	void operator delete (void* p, size_t size)
	{
		g_object_factory->memory_release (p, size);
	}

	void* operator new (size_t size, void* p)
	{
		return p;
	}

	void operator delete (void*, void*)
	{}
};

}
}

#endif
