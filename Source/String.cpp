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
		return (C*)::Nirvana::StdString::heap ()->allocate (0, ((size_t)len + 1) * sizeof (C), ::Nirvana::Memory::EXACTLY);
	}

	static C* dup (const C* s) NIRVANA_NOEXCEPT
	{
		if (s)
			return (C*)::Nirvana::StdString::heap ()->copy (0, (C*)s, (std::char_traits <C>::length (s) + 1) * sizeof (C), ::Nirvana::Memory::EXACTLY);
		else
			return nullptr;
	}

	static void free (C* s) NIRVANA_NOEXCEPT
	{
		if (s)
			::Nirvana::StdString::heap ()->release (s, (std::char_traits <C>::length (s) + 1) * sizeof (C));
	}
};

}

char* string_alloc (uint32_t len)
{
	return Nirvana::StringAllocator <char>::allocate (len);
}

char* string_dup (const char* s)
{
	return Nirvana::StringAllocator <char>::dup (s);
}

void string_free (char* s)
{
	Nirvana::StringAllocator <char>::free (s);
}

wchar_t* wstring_alloc (uint32_t len)
{
	return Nirvana::StringAllocator <wchar_t>::allocate (len);
}

wchar_t* wstring_dup (const wchar_t* s)
{
	return Nirvana::StringAllocator <wchar_t>::dup (s);
}

void wstring_free (wchar_t* s)
{
	Nirvana::StringAllocator <wchar_t>::free (s);
}

}

#endif

