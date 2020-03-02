#ifndef NIRVANA_ORB_CORE_LOADER_H_
#define NIRVANA_ORB_CORE_LOADER_H_

#include <CORBA/CORBA.h>
#include <llvm/BinaryFormat/COFF.h>
#include <list>
#include <map>

namespace TestORB {

class Loader
{
public:
	Loader ();
	~Loader ();

private:

	void bind (const void* image_base, const llvm::COFF::header* hdr);
	void bind_olf (const void* data, size_t size);

	static bool is_section (const llvm::COFF::section* s, const char* name);

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
	std::list <CORBA::Nirvana::Interface_var> core_objects_;
	std::map <NameKey, CORBA::Nirvana::Interface_var> exported_interfaces_;
};

}

#endif
