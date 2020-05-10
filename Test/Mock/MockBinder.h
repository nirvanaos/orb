#ifndef NIRVANA_TEST_MOCKBINDER_H_
#define NIRVANA_TEST_MOCKBINDER_H_

#include <Nirvana/Nirvana.h>
#include <Nirvana/Binder_s.h>
#include <Core/ModuleInfo.h>
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
	class Iterator
	{
	public:
		Iterator (const void* data, size_t size) :
			cur_ptr_ ((OLF_Command*)data),
			end_ ((OLF_Command*)((uint8_t*)data + size))
		{
			check ();
		}

		bool end () const
		{
			return cur_ptr_ == end_;
		}

		OLF_Command* cur () const
		{
			return cur_ptr_;
		}

		void next ()
		{
			if (!end ()) {
				Word idx = (Word)(*cur_ptr_) - 1;
				assert (idx >= 0);
				cur_ptr_ = (OLF_Command*)((uint8_t*)cur_ptr_ + command_sizes_ [idx]);
				check ();
			}
		}

	private:
		void check ();

	private:
		OLF_Command* cur_ptr_;
		OLF_Command* end_;

		static const size_t command_sizes_ [OLF_IMPORT_OBJECT];
	};

	struct Module;
	
	void module_bind (Module& module);
	void module_unbind (Module& module);

	void add_export (Module& module, const char* name, CORBA::Nirvana::Interface_ptr itf);

	class NameKey
	{
	public:
		NameKey (const char* name) :
			name_ (name),
			len_ (strlen (name))
		{}

		NameKey (const std::string& s) :
			name_ (s.c_str ()),
			len_ (s.length ())
		{}

		bool operator < (const NameKey& rhs) const
		{
			return CORBA::Nirvana::RepositoryId::compare (name_, len_, rhs.name_, rhs.len_) < 0;
		}

		bool compatible (const std::string& s) const
		{
			return CORBA::Nirvana::RepositoryId::compatible (name_, len_, s);
		}

	private:
		const char* name_;
		const size_t len_;
	};

private:
	CORBA::ULong ref_cnt_;

	typedef std::map <NameKey, CORBA::Nirvana::Interface_ptr> ExportedInterfaces;
	ExportedInterfaces exported_interfaces_;

	struct Module : Core::ModuleInfo
	{
		std::vector <ExportedInterfaces::iterator> interfaces_;
	} module_;
};

}
}

#endif
