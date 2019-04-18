#ifndef NIRVANA_TESTORB_LOADER_H_
#define NIRVANA_TESTORB_LOADER_H_

#include <CORBA/OLF.h>
#include <CORBA/RepositoryId.h>
#include <llvm/BinaryFormat/COFF.h>

namespace CORBA {
namespace Nirvana {
namespace OLF {

class Binder
{
public:
	Binder ();
	~Binder ();

private:

	void bind (const void* image_base, const llvm::COFF::header* hdr);
	void bind_olf (const void* data, size_t size);

	static bool is_section (const llvm::COFF::section* s, const char* name);

	const SectionHeader* next_sibling (const SectionHeader* ps)
	{
		return (const SectionHeader*)((const uint8_t*)(ps + 1) + ps->size);
	}

	template <class I>
	static T_ptr <I> get_interface (const ExportInterface& ei)
	{
		if (RepositoryId::compatible (ei.itf->_epv ().interface_id, Bridge <I>::interface_id_))
			return static_cast <Bridge <I>*> (ei.itf);
		else
			throw INV_OBJREF ();
	}

private:
	const ObjectLink* links_, *links_end_;
};

}
}
}

#endif
