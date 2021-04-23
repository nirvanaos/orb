/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include <CORBA/String_compat.h>

namespace CORBA {
namespace Nirvana {

template <typename C>
class StringAllocator
{
public:
	static C* allocate (uint32_t len) NIRVANA_NOEXCEPT
	{
		return (C*)::Nirvana::g_memory->allocate (0, ((size_t)len + 1) * sizeof (C), ::Nirvana::Memory::EXACTLY);
	}

	static C* dup (const C* s) NIRVANA_NOEXCEPT
	{
		if (s)
			return (C*)::Nirvana::g_memory->copy (0, (C*)s, (std::char_traits <C>::length (s) + 1) * sizeof (C), ::Nirvana::Memory::EXACTLY);
		else
			return nullptr;
	}

	static void free (C* s) NIRVANA_NOEXCEPT
	{
		if (s)
			::Nirvana::g_memory->release (s, (std::char_traits <C>::length (s) + 1) * sizeof (C));
	}
};

}

Char* string_alloc (ULong len)
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

WChar* wstring_alloc (ULong len)
{
	return Nirvana::StringAllocator <WChar>::allocate (len);
}

WChar* wstring_dup (const WChar* s)
{
	return Nirvana::StringAllocator <WChar>::dup (s);
}

void wstring_free (WChar* s)
{
	Nirvana::StringAllocator <WChar>::free (s);
}

}

