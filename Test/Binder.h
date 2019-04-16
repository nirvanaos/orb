#ifndef NIRVANA_TESTORB_LOADER_H_
#define NIRVANA_TESTORB_LOADER_H_

#include <CORBA/OLF.h>

namespace CORBA {
namespace Nirvana {
namespace OLF {

class Binder
{
public:
	Binder ();
	~Binder ();

private:
	void bind_coff (const void* image_base, const void* coff);
	void bind_olf (const void* data, size_t size);

	const SectionHeader* next_sibling (const SectionHeader* ps)
	{
		return (const SectionHeader*)((const uint8_t*)(ps + 1) + ps->size);
	}

private:
	const ObjectLink* links_, *links_end_;
};

}
}
}

#endif
