#include <CORBA/String.h>

#ifdef LEGACY_STRING_MAPPING_SUPPORT

namespace CORBA {
namespace Nirvana {

template <typename C>
class StringAllocator
{
public:
	static C* allocate (uint32_t len) NIRVANA_NOEXCEPT
	{
		return (C*)::Nirvana::g_default_heap->allocate (0, ((size_t)len + 1) * sizeof (C), ::Nirvana::Memory::EXACTLY);
	}

	static C* dup (const C* s) NIRVANA_NOEXCEPT
	{
		if (s)
			return (C*)::Nirvana::g_default_heap->copy (0, (C*)s, (std::char_traits <C>::length (s) + 1) * sizeof (C), ::Nirvana::Memory::EXACTLY);
		else
			return nullptr;
	}

	static void free (C* s) NIRVANA_NOEXCEPT
	{
		if (s)
			::Nirvana::g_default_heap->release (s, (std::char_traits <C>::length (s) + 1) * sizeof (C));
	}
};

}

Char* string_alloc (uint32_t len)
{
	return Nirvana::StringAllocator <Char>::allocate (len);
}

Char* string_dup (const Char* s)
{
	return Nirvana::StringAllocator <Char>::dup (s);
}

void string_free (Char* s)
{
	Nirvana::StringAllocator <char>::free (s);
}

WChar* wstring_alloc (uint32_t len)
{
	return Nirvana::StringAllocator <WChar>::allocate (len);
}

WChar* wstring_dup (const wchar_t* s)
{
	return Nirvana::StringAllocator <WChar>::dup (s);
}

void wstring_free (WChar* s)
{
	Nirvana::StringAllocator <WChar>::free (s);
}

}

#endif

