#ifndef NIRVANA_TEST_MOCKBINDER_H_
#define NIRVANA_TEST_MOCKBINDER_H_

#include <Nirvana/Nirvana.h>
#include <Nirvana/Binder_s.h>
#include <llvm/BinaryFormat/COFF.h>
#include <list>
#include <map>
#include <gtest/gtest.h>

namespace Nirvana {
namespace Test {

class MockBinder :
	public ::CORBA::Nirvana::Servant <MockBinder, ::Nirvana::Binder>,
	public ::CORBA::Nirvana::LifeCycleRefCnt <MockBinder>
{
public:
	MockBinder ();
	~MockBinder ();

	void _add_ref ()
	{
		++ref_cnt_;
	}

	void _remove_ref ()
	{
		ASSERT_GT (ref_cnt_, 1u);
		--ref_cnt_;
	}

	CORBA::ULong _refcount_value ()
	{
		return ref_cnt_;
	}

	CORBA::Nirvana::Interface_var bind (const std::string& name, const std::string& iid);

private:

	static bool is_section (const llvm::COFF::section* s, const char* name);

	void bind_image (const void* image_base, const llvm::COFF::header* hdr);
	void bind_olf (const void* data, size_t size);

	template <class I>
	void add_export (const char* name, CORBA::Nirvana::I_var <I>& core_obj)
	{
		CORBA::Nirvana::Interface_ptr proxy = CORBA::AbstractBase_ptr (core_obj)->_query_interface (CORBA::Object::repository_id_);
		if (!proxy)
			throw_OBJ_ADAPTER ();
		auto ins = exported_interfaces_.emplace (name, proxy);
		if (!ins.second)
			throw_INV_OBJREF ();	// Duplicated name
		try {
			module_.core_objects.emplace_back (core_obj._retn ());
		} catch (...) {
			exported_interfaces_.erase (ins.first);
			throw;
		}
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
	CORBA::ULong ref_cnt_;
	
	std::map <NameKey, CORBA::Nirvana::Interface_ptr> exported_interfaces_;

	struct Module
	{
		// Release bound interfaces firts, then delete core objects.
		std::list <CORBA::Nirvana::Interface_var> bound_interfaces;
		std::list <CORBA::Nirvana::Interface_var> core_objects;

		void clear ()
		{
			bound_interfaces.clear ();
			core_objects.clear ();
		}
	} module_;
};

}
}

#endif
