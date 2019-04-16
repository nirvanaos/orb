#include "Binder.h"
#include <CORBA/ObjectFactory_c.h>
#include <Windows.h>

namespace COFF {

// Sizes in bytes of various things in the COFF format.
enum
{
	Header16Size = 20,
	Header32Size = 56,
	NameSize = 8,
	Symbol16Size = 18,
	Symbol32Size = 20,
	SectionSize = 40,
	RelocationSize = 10
}; 

struct header
{
	uint16_t Machine;
	uint16_t NumberOfSections;
	uint32_t TimeDateStamp;
	uint32_t PointerToSymbolTable;
	uint32_t NumberOfSymbols;
	uint16_t SizeOfOptionalHeader;
	uint16_t Characteristics;
};

struct section
{
	char     Name [NameSize];
	uint32_t VirtualSize;
	uint32_t VirtualAddress;
	uint32_t SizeOfRawData;
	uint32_t PointerToRawData;
	uint32_t PointerToRelocations;
	uint32_t PointerToLineNumbers;
	uint16_t NumberOfRelocations;
	uint16_t NumberOfLineNumbers;
	uint32_t Characteristics;
};

const uint16_t MZ_SIGN = 'ZM';
const uint32_t PE_SIGN = 'EP';
const size_t PE_OFFSET = 0x3C;

}

namespace CORBA {
namespace Nirvana {
namespace OLF {

bool is_section (const COFF::section* s, const char* name)
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
	const uint8_t* p = (const uint8_t*)image_base;
	if (COFF::MZ_SIGN != *(const uint16_t*)p)
		throw INITIALIZE ();
	p += *(const uint32_t*)(p + COFF::PE_OFFSET);
	if (COFF::PE_SIGN != *(const uint32_t*)p)
		throw INITIALIZE ();
	bind_coff (image_base, p + 4);
}

Binder::~Binder ()
{
	for (const ObjectLink* p = links_; p != links_end_; ++p)
		CORBA::release (p->interface_ptr);
}

void Binder::bind_coff (const void* image_base, const void* coff)
{
	const COFF::section* metadata = nullptr;

	const COFF::header* hdr = (const COFF::header*)coff;
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
	if (ps < end && SectionType::OBJECT_EXPORT == ps->type)
		ps = next_sibling (ps);

	ObjectLink* links_begin = nullptr, *links_end = nullptr;
	if (ps < end && SectionType::OBJECT_LINK == ps->type) {
		links_begin = (ObjectLink*)(ps + 1);
		ps = next_sibling (ps);
		links_end = (ObjectLink*)(ps);
	}

	if (ps < end && links_begin >= links_end)
		throw INITIALIZE ();

	DWORD protection;
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

		case SectionType::STATIC_OBJECT:
			for (const StaticObject* p = (const StaticObject*)(ps + 1), *end = (const StaticObject*)next_sibling (ps); p < end; ++p) {
				if (link_ptr >= links_end)
					throw INITIALIZE ();
				(link_ptr++)->interface_ptr = g_object_factory->create_servant (p->servant, DynamicServant_ptr::nil ());
				links_end_ = link_ptr;
			}
			break;

		case SectionType::STATIC_LOCAL:
			for (const StaticLocal* p = (const StaticLocal*)(ps + 1), *end = (const StaticLocal*)next_sibling (ps); p < end; ++p) {
				if (link_ptr >= links_end)
					throw INITIALIZE ();
				(link_ptr++)->interface_ptr = Object_ptr (g_object_factory->create_local_object (p->servant, DynamicServant_ptr::nil ()));
				links_end_ = link_ptr;
			}
			break;

		case SectionType::OBJECT_IMPORT:
			throw NO_IMPLEMENT ();

		default:
			throw INITIALIZE ();
		}

		ps = next_sibling (ps);
		if (sync_domain_end && ps >= sync_domain_end)
			sync_domain_end = nullptr;
	}

	VirtualProtect (links_begin, (links_end - links_begin) * sizeof (ObjectLink), protection, &protection);
}

}
}
}
