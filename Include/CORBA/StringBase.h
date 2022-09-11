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
#ifndef NIRVANA_ORB_STRINGBASE_H_
#define NIRVANA_ORB_STRINGBASE_H_
#pragma once

#include "ABI_String.h"
#include "primitive_types.h"
#include <type_traits>
#include <string.h>
#include <wchar.h>
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Internal {

template <typename C>
size_t string_len (const C* s);

template <> inline
size_t string_len (const Char* s)
{
	return strlen (s);
}

template <> inline
size_t string_len (const WChar* s)
{
	const WChar* p = s;
	while (*p)
		++p;
	return p - s;
}

template <typename C, ULong bound = 0>
class StringView
{
	typedef ABI <StringT <C> > ABI;

	template <typename C, ULong bound>
	friend class StringView;

public:
	StringView (const StringView& s) NIRVANA_NOEXCEPT
	{
		size_t size;
		const C* p;
		if (s.abi_.is_large ()) {
			size = s.abi_.large_size ();
			p = s.abi_.large_pointer ();
		} else {
			size = s.abi_.small_size ();
			p = s.abi_.small_pointer ();
		}
		abi_.large_pointer (const_cast <C*> (p));
		abi_.large_size (size);
		abi_.allocated (0);
	}

	template <ULong bound1>
	StringView (const StringView <C, bound1>& s)
	{
		size_t size;
		const C* p;
		if (s.abi_.is_large ()) {
			size = s.abi_.large_size ();
			p = s.abi_.large_pointer ();
		} else {
			size = s.abi_.small_size ();
			p = s.abi_.small_pointer ();
		}
		if (bound && (!bound1 || bound1 > bound) && size > bound)
			Nirvana::throw_BAD_PARAM ();
		abi_.large_pointer (const_cast <C*> (p));
		abi_.large_size (size);
		abi_.allocated (0);
	}

#ifdef NIRVANA_C11
	template <size_t cc>
	StringView (C const (&s) [cc])
	{
		if (bound && cc - 1 > bound)
			Nirvana::throw_BAD_PARAM ();
		abi_.large_pointer (const_cast <C*> (s));
		abi_.large_size (cc - 1);
		abi_.allocated (0);
	}

	template <size_t cc>
	StringView (C (&s) [cc]) :
		StringView ((const C*)s)
	{}

	template <typename S, typename = typename std::enable_if <std::is_convertible <S, const C*>::value && !std::is_array <S>::value>::type>
	StringView (S s);
#else
	StringView (const C* p);
#endif

	template <class A, typename = std::enable_if_t <!std::is_same <A, std::allocator <C> >::value> >
	StringView (const std::basic_string <C, std::char_traits <C>, A>&);

	StringView (const C* p, size_t cc);

	StringView (nullptr_t)
	{
		abi_.reset ();
	}

	const ABI* operator & () const
	{
		return &abi_;
	}

	bool empty () const
	{
		return abi_.empty ();
	}

	operator const StringT <C>& () const NIRVANA_NOEXCEPT
	{
		return reinterpret_cast <const StringT <C>&> (abi_);
	}

protected:
	StringView ()
	{}

private:
	static size_t _length (const C* s)
	{
		return string_len (s);
	}

private:
	ABI abi_;
};

#ifdef NIRVANA_C11

template <typename C, ULong bound>
template <typename S, typename>
StringView <C, bound>::StringView (S s)
{
	const C* p = s;
	size_t cc;
	if (p && (cc = _length (p))) {
		if (bound && cc > bound)
			Nirvana::throw_BAD_PARAM ();
		abi_.large_pointer (const_cast <C*> (p));
		abi_.large_size (cc);
		abi_.allocated (0);
	} else
		abi_.reset ();
}

#else

template <typename C, ULong bound>
template <typename S, typename>
StringView <C, bound>::StringView (const C* p)
{
	size_t cc;
	if (p && (cc = _length (p))) {
		if (bound && cc > bound)
			Nirvana::throw_BAD_PARAM ();
		abi_.large_pointer (const_cast <C*> (p));
		abi_.large_size (cc);
		abi_.allocated (0);
	} else
		abi_.reset ();
}

#endif

template <typename C, ULong bound>
StringView <C, bound>::StringView (const C* p, size_t cc)
{
	if (p && cc) {
		if (bound && cc > bound)
			Nirvana::throw_BAD_PARAM ();
		abi_.large_pointer (const_cast <C*> (p));
		abi_.large_size (cc);
		abi_.allocated (0);
	} else
		abi_.reset ();
}

typedef const StringView <Char>& String_in;
typedef const StringView <WChar>& WString_in;

/// Unbounded string. Equivalent to std::string.
typedef StringT <Char> String;

/// Unbounded wide string. Equivalent to std::wstring.
typedef StringT <WChar> WString;

}
}

namespace IDL {

/// Unbounded string. Equivalent to std::string.
typedef CORBA::Internal::String String;

/// Unbounded wide string. Equivalent to std::wstring.
typedef CORBA::Internal::WString WString;


}

#endif
