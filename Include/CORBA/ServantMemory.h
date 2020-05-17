#ifndef NIRVANA_ORB_SERVANTMEMORY_H_
#define NIRVANA_ORB_SERVANTMEMORY_H_

#include "core_objects.h"

namespace CORBA {
namespace Nirvana {

class ServantMemory
{
public:
#ifdef NIRVANA_C14

	void* operator new (size_t size)
	{
		return g_object_factory->memory_allocate (size);
	}

	void operator delete (void* p, size_t size)
	{
		g_object_factory->memory_release (p, size);
	}

#else

	void* operator new (size_t size)
	{
		size += sizeof (size_t);
		size_t* hdr = (size_t*)g_object_factory->memory_allocate (size);
		*hdr = size;
		return hdr + 1;
	}

	void operator delete (void* p)
	{
		size_t* hdr = (size_t*)p - 1;
		g_object_factory->memory_release (hdr, *hdr);
	}

#endif

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
