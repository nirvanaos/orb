#include "MockBinder.h"
#include "ObjectFactory.h"
#include <Nirvana/OLF.h>
#include <Nirvana/core_objects.h>
#include "Core/ORB/POA.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#endif

namespace Nirvana {

NIRVANA_OLF_SECTION
const ImportInterfaceT <Binder> g_binder = { OLF_IMPORT_INTERFACE, "Nirvana/g_binder", Binder::repository_id_ };

namespace Test {

using namespace std;
using namespace ::CORBA;
using namespace ::CORBA::Nirvana;
using namespace ::Nirvana;
using ::CORBA::Nirvana::Core::ObjectFactory;

const size_t MockBinder::Iterator::command_sizes_ [OLF_IMPORT_OBJECT] = {
	sizeof (ImportInterface),
	sizeof (ExportInterface),
	sizeof (ExportObject),
	sizeof (ExportLocal),
	sizeof (ImportInterface)
};

MockBinder::MockBinder () :
	ref_cnt_ (1)
{
	exported_interfaces_.emplace (g_binder.imp.name, _get_ptr ());

	module_.initialize (GetModuleHandleW (0));

	module_bind (module_);
}

MockBinder::~MockBinder ()
{
	module_unbind (module_);
	interface_release (CORBA::Nirvana::Core::g_root_POA.itf);
	EXPECT_EQ (ref_cnt_, 1);
//	EXPECT_EQ (module_.ref_cnt_, 1);
}

void MockBinder::module_bind (Module& module)
{
#ifdef _WIN32
	DWORD protection;
	VirtualProtect ((void*)module.olf_section, module.olf_size, PAGE_READWRITE, &protection);
#else
	mprotect (olf_section, module.olf_size, PROT_READ | PROT_WRITE);
#endif

	try {

		// Pass 0: Bind g_module
		ImportInterface* module_entry = nullptr;
		for (Iterator it (module.olf_section, module.olf_size); !it.end (); it.next ()) {
			if (OLF_IMPORT_INTERFACE == *it.cur ()) {
				ImportInterface* ps = reinterpret_cast <ImportInterface*> (it.cur ());
				if (!strcmp (ps->name, "Nirvana/g_module")) {
					if (!CORBA::Nirvana::RepositoryId::compatible (Module::repository_id_, ps->interface_id))
						throw_INV_OBJREF ();
					ps->itf = &module_;
					module_entry = ps;
					break;
				}
			}
		}

		// Pass 1: Export pseudo objects.
		for (Iterator it (module.olf_section, module.olf_size); !it.end (); it.next ()) {
			if (OLF_EXPORT_INTERFACE  == *it.cur ()) {
				const ExportInterface* ps = reinterpret_cast <const ExportInterface*> (it.cur ());
				add_export (module, ps->name, ps->itf);
			}
		}

		// Pass 2: Import pseudo objects.
		for (Iterator it (module.olf_section, module.olf_size); !it.end (); it.next ()) {
			if (OLF_IMPORT_INTERFACE  == *it.cur ()) {
				ImportInterface* ps = reinterpret_cast <ImportInterface*> (it.cur ());
				if (ps != module_entry)
					ps->itf = &bind (ps->name, ps->interface_id)._retn ();
			}
		}

		// Create POA
		PortableServer::Servant_var <CORBA::Nirvana::Core::POA> poa = new CORBA::Nirvana::Core::POA;
		CORBA::Nirvana::Core::g_root_POA = poa->_this ();

		// Pass 3: Export objects.
		for (Iterator it (module.olf_section, module.olf_size); !it.end (); it.next ()) {
			switch (*it.cur ()) {
				case OLF_EXPORT_OBJECT: {
					ExportObject* ps = reinterpret_cast <ExportObject*> (it.cur ());
					PortableServer::ServantBase_var core_obj = ObjectFactory::create_servant (TypeI <PortableServer::ServantBase>::in (ps->servant_base));
					Object_ptr obj = AbstractBase_ptr (core_obj)->_query_interface <Object> ();
					ps->core_object = &core_obj._retn ();
					add_export (module, ps->name, obj);
					break;
				}

				case OLF_EXPORT_LOCAL: {
					ExportLocal* ps = reinterpret_cast <ExportLocal*> (it.cur ());
					LocalObject_ptr core_obj = ObjectFactory::create_local_object (TypeI <LocalObject>::in (ps->local_object), TypeI <AbstractBase>::in (ps->abstract_base));
					Object_ptr obj = core_obj;
					ps->core_object = &core_obj;
					add_export (module, ps->name, obj);
					break;
				}
			}
		}

		// Pass 4: Import objects.
		for (Iterator it (module.olf_section, module.olf_size); !it.end (); it.next ()) {
			if (OLF_IMPORT_OBJECT  == *it.cur ()) {
				ImportInterface* ps = reinterpret_cast <ImportInterface*> (it.cur ());
				ps->itf = &bind (ps->name, ps->interface_id)._retn ();
			}
		}
	} catch (...) {
		module_unbind (module);
		VirtualProtect ((void*)module.olf_section, module.olf_size, protection, &protection);
		throw;
	}

#ifdef _WIN32
	VirtualProtect ((void*)module.olf_section, module.olf_size, protection, &protection);
#else
	mprotect (olf_section, module.olf_size, PROT_READ);
#endif
}

void MockBinder::module_unbind (Module& module)
{
	// Pass 1
	for (Iterator it (module.olf_section, module.olf_size); !it.end (); it.next ()) {
		switch (*it.cur ()) {
			case OLF_IMPORT_INTERFACE: {
				ImportInterface* ps = reinterpret_cast <ImportInterface*> (it.cur ());
				CORBA::Nirvana::interface_release (ps->itf);
				break;
			}
		}
	}

	// Pass 2
	for (Iterator it (module.olf_section, module.olf_size); !it.end (); it.next ()) {
		switch (*it.cur ()) {
			case OLF_EXPORT_OBJECT: {
				ExportObject* ps = reinterpret_cast <ExportObject*> (it.cur ());
				CORBA::Nirvana::interface_release (ps->core_object);
				break;
			}

			case OLF_EXPORT_LOCAL: {
				ExportLocal* ps = reinterpret_cast <ExportLocal*> (it.cur ());
				CORBA::Nirvana::interface_release (ps->core_object);
				break;
			}
		}
	}
}

void MockBinder::add_export (Module& module, const char* name, CORBA::Nirvana::Interface_ptr itf)
{
	auto ins = exported_interfaces_.emplace (name, itf);
	if (!ins.second)
		throw_INV_OBJREF ();	// Duplicated name
	try {
		module_.interfaces_.push_back (ins.first);
	} catch (...) {
		exported_interfaces_.erase (ins.first);
		throw;
	}
}

Interface_var MockBinder::bind (const std::string& name, const std::string& iid)
{
	auto pf = exported_interfaces_.lower_bound (name);
	if (pf != exported_interfaces_.end () && pf->first.compatible (name)) {
		Interface* itf = &pf->second;
		const StringBase <Char> itf_id = itf->_epv ().interface_id;
		if (!::CORBA::Nirvana::RepositoryId::compatible (itf_id, iid)) {
			AbstractBase_ptr ab = AbstractBase::_nil ();
			if (::CORBA::Nirvana::RepositoryId::compatible (itf_id, Object::repository_id_))
				ab = Object_ptr (static_cast <Object*> (itf));
			if (::CORBA::Nirvana::RepositoryId::compatible (itf_id, AbstractBase::repository_id_))
				ab = static_cast <AbstractBase*> (itf);
			else
				throw INV_OBJREF ();
			Interface* qi = ab->_query_interface (iid);
			if (!qi)
				throw INV_OBJREF ();
			itf = qi;
		}

		return Interface_ptr (interface_duplicate (itf));

	} else
		throw OBJECT_NOT_EXIST ();
}

void MockBinder::Iterator::check ()
{
	if (cur_ptr_ >= end_)
		cur_ptr_ = end_;
	else {
		Word cmd = *cur_ptr_;
		if (OLF_END == cmd)
			cur_ptr_ = end_;
		else if (cmd > OLF_IMPORT_OBJECT)
			throw CORBA::INITIALIZE ();
	}
}

}
}
