#include "Loader.h"
#include "ObjectFactoryCore.h"
#include <Nirvana/OLF.h>
#include <Windows.h>

namespace TestORB {

using namespace llvm;
using ::Nirvana::Word;
using namespace std;
using namespace ::CORBA;
using namespace ::CORBA::Nirvana;
using namespace ::Nirvana;

bool Loader::is_section (const COFF::section* s, const char* name)
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

Loader::Loader ()
{
	void* image_base = GetModuleHandleW (nullptr);
	const COFF::DOSHeader* dos_header = (const COFF::DOSHeader*)image_base;
	if (dos_header->Magic != 'ZM')
		throw INITIALIZE ();

	const uint32_t* PE_magic = (const uint32_t*)((const uint8_t*)image_base + dos_header->AddressOfNewExeHeader);
	if (*PE_magic != *(const uint32_t*)COFF::PEMagic)
		throw INITIALIZE ();

	bind (image_base, (COFF::header*)(PE_magic + 1));
}

Loader::~Loader ()
{}

void Loader::bind (const void* image_base, const COFF::header* hdr)
{
	const COFF::section* metadata = nullptr;

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

void Loader::bind_olf (const void* data, size_t size)
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
				exported_interfaces_.emplace (ps->name, static_cast <Interface*> (Interface::__duplicate (ps->itf)));
				p += sizeof (ExportInterface) / sizeof (*p);
				break;
			}

			case OLF_EXPORT_OBJECT: {
				ExportObject* ps = reinterpret_cast <ExportObject*> (p);
				PortableServer::ServantBase_var core_obj = ObjectFactoryCore::create_servant (_unmarshal_in (ps->implementation), DynamicServant_ptr::nil ());
				ps->core_object = core_obj;
				Object_var proxy = Object::_duplicate (static_cast <Object*> (AbstractBase_ptr (core_obj)->_query_interface (Object::interface_id_)));
				core_objects_.push_back (Interface_var (core_obj._retn ()));
				exported_interfaces_.emplace (ps->name, Interface_var (proxy._retn ()));
				p += sizeof (ExportObject) / sizeof (*p);
				break;
			}

			case OLF_EXPORT_LOCAL: {
				ExportLocal* ps = reinterpret_cast <ExportLocal*> (p);
				LocalObject_var core_obj = ObjectFactoryCore::create_local_object (_unmarshal_in (ps->implementation), DynamicServant_ptr::nil ());
				ps->core_object = core_obj;
				LocalObject_var proxy = LocalObject::_duplicate (static_cast <LocalObject*> (AbstractBase_ptr (Object_ptr (core_obj))->_query_interface (LocalObject::interface_id_)));
				core_objects_.push_back (Interface_var (core_obj._retn ()));
				exported_interfaces_.emplace (ps->name, Interface_var (proxy._retn ()));
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
					auto pf = exported_interfaces_.find (ps->name);
					if (pf != exported_interfaces_.end ()) {
						Bridge <Interface>* itf = pf->second;
						const Char* itf_id = itf->_epv ().interface_id;
						if (RepositoryId::compatible (itf_id, ps->interface_id))
							ps->itf = itf;
						else {
							AbstractBase_ptr ab = AbstractBase::_nil ();
							if (RepositoryId::compatible (itf_id, AbstractBase::interface_id_))
								ab = static_cast <AbstractBase*> (itf);
							else if (RepositoryId::compatible (itf_id, Object::interface_id_))
								ab = Object_ptr (static_cast <Object*> (itf));
							else if (RepositoryId::compatible (itf_id, LocalObject::interface_id_))
								ab = Object_ptr (LocalObject_ptr (static_cast <LocalObject*> (itf)));
							else
								throw INV_OBJREF ();
							Bridge <Interface>* qi = ab->_query_interface (ps->interface_id);
							if (!qi)
								throw INV_OBJREF ();
							ps->itf = qi;
						}
					} else
						throw OBJECT_NOT_EXIST ();
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

}
