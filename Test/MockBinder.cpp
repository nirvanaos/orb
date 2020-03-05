#include "MockBinder.h"
#include "Core/ORB/ObjectFactory.h"
#include <Nirvana/OLF.h>
#include <Nirvana/core_objects.h>

// TODO: Make mock portable
#include <Windows.h>

namespace Nirvana {

__declspec (allocate(OLF_BIND))
const ImportInterfaceT <Binder> g_binder = { OLF_IMPORT_INTERFACE, "Nirvana/g_binder", Binder::interface_id_ };

namespace Test {

using namespace llvm;
using ::Nirvana::Word;
using namespace std;
using namespace ::CORBA;
using namespace ::CORBA::Nirvana;
using namespace ::Nirvana;
using ::CORBA::Nirvana::Core::ObjectFactory;

bool MockBinder::is_section (const COFF::section* s, const char* name)
{
	const char* sn = s->Name;
	char c;
	while ((c = *name) && *sn == c) {
		++sn;
		++name;
	}
	if (c)
		return false;
	if (sn - s->Name < COFF::NameSize)
		return !*sn;
	else
		return true;
}

MockBinder::MockBinder () :
	ref_cnt_ (1)
{
	exported_interfaces_.emplace (g_binder.imp.name, _get_ptr ());

	void* image_base = GetModuleHandleW (0);
	const COFF::DOSHeader* dos_header = (const COFF::DOSHeader*)image_base;
	if (dos_header->Magic != 'ZM')
		throw INITIALIZE ();

	const uint32_t* PE_magic = (const uint32_t*)((const uint8_t*)image_base + dos_header->AddressOfNewExeHeader);
	if (*PE_magic != *(const uint32_t*)COFF::PEMagic)
		throw INITIALIZE ();

	bind_image (image_base, (COFF::header*)(PE_magic + 1));
}

MockBinder::~MockBinder ()
{
	module_.clear ();
	EXPECT_EQ (ref_cnt_, 1);
}

void MockBinder::bind_image (const void* image_base, const COFF::header* hdr)
{
	const COFF::section* metadata = 0;

	const COFF::section* s = (const COFF::section*)((const uint8_t*)(hdr + 1) + hdr->SizeOfOptionalHeader);
	for (uint32_t cnt = hdr->NumberOfSections; cnt; --cnt, ++s) {
		if (is_section (s, OLF_BIND)) {
			metadata = s;
			break;
		}
	}

	if (!metadata)
		throw INITIALIZE ();

	bind_olf ((const uint8_t*)image_base + metadata->VirtualAddress, metadata->VirtualSize);
}

void MockBinder::bind_olf (const void* data, size_t size)
{
	DWORD protection;
	VirtualProtect ((void*)data, size, PAGE_READWRITE, &protection);
	Word* p = (Word*)data;
	Word* end = p + size / sizeof (Word);

	bool imp = false;
	while (p != end && *p) {
		switch (*p) {
			case OLF_IMPORT_INTERFACE:
				imp = true;
				p += sizeof (ImportInterface) / sizeof (*p);
				break;

			case OLF_EXPORT_INTERFACE: {
				const ExportInterface* ps = reinterpret_cast <const ExportInterface*> (p);
				if (!exported_interfaces_.emplace (ps->name, ps->itf).second)
					throw INV_OBJREF ();	// Duplicated name
				p += sizeof (ExportInterface) / sizeof (*p);
				break;
			}

			case OLF_EXPORT_OBJECT: {
				ExportObject* ps = reinterpret_cast <ExportObject*> (p);
				PortableServer::ServantBase_var core_obj = ObjectFactory::create_servant (TypeI <PortableServer::ServantBase>::in (ps->implementation));
				ps->core_object = PortableServer::Servant (core_obj);
				add_export (ps->name, core_obj);
				p += sizeof (ExportObject) / sizeof (*p);
				break;
			}

			case OLF_EXPORT_LOCAL: {
				ExportLocal* ps = reinterpret_cast <ExportLocal*> (p);
				Object_var core_obj = ObjectFactory::create_local_object (TypeI <Object>::in (ps->implementation));
				ps->core_object = Object_ptr (core_obj);
				add_export (ps->name, core_obj);
				p += sizeof (ExportLocal) / sizeof (*p);
				break;
			}

			default:
				throw INTERNAL ();
				break;
		}
	}
	if (imp) {
		p = (Word*)data;
		while (p != end && *p) {
			switch (*p) {
				case OLF_IMPORT_INTERFACE: {
					ImportInterface* ps = reinterpret_cast <ImportInterface*> (p);
					module_.bound_interfaces.push_back (bind (ps->name, ps->interface_id));
					ps->itf = Interface_ptr (module_.bound_interfaces.back ());
					p += sizeof (ImportInterface) / sizeof (*p);
					break;
				}

				case OLF_EXPORT_INTERFACE:
					p += sizeof (ExportInterface) / sizeof (*p);
					break;

				case OLF_EXPORT_OBJECT:
					p += sizeof (ExportObject) / sizeof (*p);
					break;

				case OLF_EXPORT_LOCAL:
					p += sizeof (ExportLocal) / sizeof (*p);
					break;
			}
		}
	}
	VirtualProtect ((void*)data, size, protection, &protection);
}

Interface_var MockBinder::bind (const std::string& name, const std::string& iid)
{
	auto pf = exported_interfaces_.find (name.c_str ());
	if (pf != exported_interfaces_.end ()) {
		Interface* itf = Interface_ptr (pf->second);
		const StringBase <Char> itf_id = itf->_epv ().interface_id;
		if (!::CORBA::Nirvana::RepositoryId::compatible (itf_id, iid)) {
			AbstractBase_ptr ab = AbstractBase::_nil ();
			if (::CORBA::Nirvana::RepositoryId::compatible (itf_id, Object::interface_id_))
				ab = Object_ptr (static_cast <Object*> (itf));
			if (::CORBA::Nirvana::RepositoryId::compatible (itf_id, AbstractBase::interface_id_))
				ab = static_cast <AbstractBase*> (itf);
			else
				throw INV_OBJREF ();
			Interface* qi = ab->_query_interface (iid);
			if (!qi)
				throw INV_OBJREF ();
			itf = qi;
		}

		return interface_duplicate (itf);

	} else
		throw OBJECT_NOT_EXIST ();
}

}
}
