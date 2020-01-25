#ifndef NIRVANA_TESTORB_LOADER_H_
#define NIRVANA_TESTORB_LOADER_H_

#include <CORBA/OLF.h>
#include <CORBA/RepositoryId.h>
#include <llvm/BinaryFormat/COFF.h>
#include <list>
#include <map>

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

	template <class I>
	static T_ptr <I> get_interface (const ExportInterface& ei)
	{
		if (RepositoryId::compatible (ei.itf->_epv ().interface_id, Bridge <I>::interface_id_))
			return static_cast <I*> (static_cast <Bridge <I>*> (ei.itf));
		else
			throw INV_OBJREF ();
	}

	class NameKey
	{
	public:
		NameKey (const char* name) :
			name_ (name),
			end_ (name + strlen (name))
		{}

		bool operator < (const NameKey& rhs) const
		{
			return std::lexicographical_compare (name_, end_, rhs.name_, rhs.end_);
		}

	private:
		const char* name_;
		const char* end_;
	};

private:
	std::list <Interface_var> core_objects_;
	std::map <NameKey, Interface_var> exported_interfaces_;
};

}
}
}

#endif
