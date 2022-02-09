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
	return wcslen (s);
}

template <typename C, ULong bound = 0>
class StringBase : protected ABI <StringT <C> >
{
public:
#ifdef NIRVANA_C11
	template <size_t cc>
	StringBase (C const (&s) [cc])
	{
		if (bound && cc - 1 > bound)
			Nirvana::throw_BAD_PARAM ();
		this->large_pointer (const_cast <C*> (s));
		this->large_size (cc - 1);
		this->allocated (0);
	}

	template <size_t cc>
	StringBase (C (&s) [cc]) :
		StringBase ((const C*)s)
	{}

	template <typename S, typename = typename std::enable_if <std::is_convertible <S, const C*>::value && !std::is_array <S>::value>::type>
	StringBase (S s);
#else
	StringBase (const C* p);
#endif

	template <class A>
	StringBase (const std::basic_string <C, std::char_traits <C>, A>&);

	StringBase (const C* p, size_t cc);

	StringBase (nullptr_t)
	{
		this->reset ();
	}

	const ABI <StringT <C> >* operator & () const
	{
		return this;
	}

	bool empty () const
	{
		return ABI <StringT <C> >::empty ();
	}

protected:
	StringBase ()
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
StringBase <C, bound>::StringBase (S s)
{
	const C* p = s;
	size_t cc;
	if (p && (cc = _length (p))) {
		if (bound && cc > bound)
			Nirvana::throw_BAD_PARAM ();
		this->large_pointer (const_cast <C*> (p));
		this->large_size (cc);
		this->allocated (0);
	} else
		this->reset ();
}

#else

template <typename C, ULong bound>
template <typename S, typename>
StringBase <C, bound>::StringBase (const C* p)
{
	size_t cc;
	if (p && (cc = _length (p))) {
		if (bound && cc > bound)
			Nirvana::throw_BAD_PARAM ();
		this->large_pointer (const_cast <C*> (p));
		this->large_size (cc);
		this->allocated (0);
	} else
		this->reset ();
}

#endif

template <typename C, ULong bound>
StringBase <C, bound>::StringBase (const C* p, size_t cc)
{
	if (p && cc) {
		if (bound && cc > bound)
			Nirvana::throw_BAD_PARAM ();
		this->large_pointer (const_cast <C*> (p));
		this->large_size (cc);
		this->allocated (0);
	} else
		this->reset ();
}

typedef const StringBase <Char>& String_in;
typedef const StringBase <WChar>& WString_in;

}
}

#endif
