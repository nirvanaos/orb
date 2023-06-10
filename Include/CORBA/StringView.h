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
#ifndef NIRVANA_ORB_STRINGVIEW_H_
#define NIRVANA_ORB_STRINGVIEW_H_
#pragma once

#include "ABI_String.h"
#include <string.h>
#include <wchar.h>
#include <type_traits>
#include "primitive_types.h"
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

/// Used as input parameter for string types.
/// 
/// \typeparam C Character type.
/// \typeparam bound String bound.
template <typename C, ULong bound = 0>
class StringView : private ABI <StringT <C> >
{
	typedef CORBA::Internal::ABI <StringT <C> > ABI;

	template <typename, ULong> friend class StringView;

public:
	StringView (const StringView& s) noexcept
	{
		size_t size;
		const C* p;
		if (s.is_large ()) {
			size = s.large_size ();
			p = s.large_pointer ();
		} else {
			size = s.small_size ();
			p = s.small_pointer ();
		}
		ABI::large_pointer (const_cast <C*> (p));
		ABI::large_size (size);
		ABI::allocated (0);
	}

	template <ULong bound1>
	StringView (const StringView <C, bound1>& s)
	{
		size_t size;
		const C* p;
		if (s.is_large ()) {
			size = s.large_size ();
			p = s.large_pointer ();
		} else {
			size = s.small_size ();
			p = s.small_pointer ();
		}
		if (bound && (!bound1 || bound1 > bound) && size > bound)
			Nirvana::throw_BAD_PARAM ();
		ABI::large_pointer (const_cast <C*> (p));
		ABI::large_size (size);
		ABI::allocated (0);
	}

#ifdef NIRVANA_C11
	template <size_t cc>
	StringView (C const (&s) [cc])
	{
		if (bound && cc - 1 > bound)
			Nirvana::throw_BAD_PARAM ();
		ABI::large_pointer (const_cast <C*> (s));
		ABI::large_size (cc - 1);
		ABI::allocated (0);
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
		ABI::reset ();
	}

	const ABI* operator & () const
	{
		return this;
	}

	bool empty () const
	{
		return ABI::empty ();
	}

	operator const StringT <C>& () const noexcept
	{
		return reinterpret_cast <const StringT <C>&> (*this);
	}

	const C* c_str () const noexcept
	{
		return ABI::_ptr ();
	}

	const C* data () const noexcept
	{
		return ABI::_ptr ();
	}

	size_t size () const noexcept
	{
		return ABI::size ();
	}

	size_t length () const noexcept
	{
		return ABI::size ();
	}

protected:
	StringView ()
	{}

private:
	static size_t _length (const C* s)
	{
		return string_len (s);
	}
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
		ABI::large_pointer (const_cast <C*> (p));
		ABI::large_size (cc);
		ABI::allocated (0);
	} else
		ABI::reset ();
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
		ABI::large_pointer (const_cast <C*> (p));
		ABI::large_size (cc);
		ABI::allocated (0);
	} else
		ABI::reset ();
}

#endif

template <typename C, ULong bound>
StringView <C, bound>::StringView (const C* p, size_t cc)
{
	if (p && cc) {
		if (bound && cc > bound)
			Nirvana::throw_BAD_PARAM ();
		ABI::large_pointer (const_cast <C*> (p));
		ABI::large_size (cc);
		ABI::allocated (0);
	} else
		ABI::reset ();
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
