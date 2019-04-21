#include "Binder.h"
#include <CORBA/ObjectFactory_c.h>
#include <Windows.h>

using namespace llvm;

namespace CORBA {
namespace Nirvana {
namespace OLF {

bool Binder::is_section (const COFF::section* s, const char* name)
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

Binder::Binder () :
	links_ (nullptr),
	links_end_ (nullptr)
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

Binder::~Binder ()
{
	for (const ObjectLink* p = links_; p != links_end_; ++p)
		CORBA::release (p->interface_ptr);
}

void Binder::bind (const void* image_base, const COFF::header* hdr)
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

void Binder::bind_olf (const void* data, size_t size)
{
	const SectionHeader* ps = (const SectionHeader*)data;
	if (SectionType::OLF != ps->type)
		throw INITIALIZE ();

	if (sizeof (SectionHeader) + ps->size > size)
		throw INITIALIZE ();

	const SectionHeader* end = next_sibling (ps);
	
	++ps; // Child

	ObjectLink* links_begin = nullptr, *links_end = nullptr;
	if (ps < end && SectionType::OBJECT_LINK == ps->type) {
		links_begin = (ObjectLink*)(ps + 1);
		ps = next_sibling (ps);
		links_end = (ObjectLink*)(ps);
	}

	DWORD protection;
	if (links_begin < links_end)
		VirtualProtect (links_begin, (links_end - links_begin) * sizeof (ObjectLink), PAGE_READWRITE, &protection);

	ObjectLink* link_ptr = links_begin;
	links_ = links_end_ = link_ptr;

	const SectionHeader* sync_domain_end = nullptr;
	while (ps < end) {
		switch (ps->type) {
		case SectionType::SYNC_DOMAIN:
			if (sync_domain_end)
				throw INITIALIZE ();
			sync_domain_end = next_sibling (ps);
			++ps;
			continue;

		case SectionType::EXPORT_INTERFACE:
			break;

		case SectionType::EXPORT_OBJECT:
			for (const ExportInterface* p = (const ExportInterface*)(ps + 1), *end = (const ExportInterface*)next_sibling (ps); p < end; ++p) {
				if (link_ptr >= links_end)
					throw INITIALIZE ();
				(link_ptr++)->interface_ptr = ObjectFactory_ptr (g_object_factory)->create_servant (get_interface <PortableServer::ServantBase> (*p), DynamicServant_ptr::nil ());
				links_end_ = link_ptr;
			}
			break;

		case SectionType::EXPORT_LOCAL:
			for (const ExportInterface* p = (const ExportInterface*)(ps + 1), *end = (const ExportInterface*)next_sibling (ps); p < end; ++p) {
				if (link_ptr >= links_end)
					throw INITIALIZE ();
				(link_ptr++)->interface_ptr = Object_ptr (ObjectFactory_ptr (g_object_factory)->create_local_object (get_interface <AbstractBase> (*p), DynamicServant_ptr::nil ()));
				links_end_ = link_ptr;
			}
			break;

		case SectionType::IMPORT_INTERFACE:
			throw NO_IMPLEMENT ();

		default:
			throw INITIALIZE ();
		}

		ps = next_sibling (ps);
		if (sync_domain_end && ps >= sync_domain_end)
			sync_domain_end = nullptr;
	}

	if (links_begin < links_end)
		VirtualProtect (links_begin, (links_end - links_begin) * sizeof (ObjectLink), protection, &protection);
}

}
}
}
