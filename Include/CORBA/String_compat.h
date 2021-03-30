/// \file
/// Provides backward compatibility with old string mapping specification.

/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_ORB_STRING_COMPAT_H_
#define NIRVANA_ORB_STRING_COMPAT_H_

#include "String.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
class String_var : public std::basic_string <C>
{
public:
	String_var ()
	{}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_var (C* s)
	{
		adopt (s);
	}

#endif

	String_var (const C* s)
	{
		size_t cc = std::char_traits <C>::length (s);
		this->assign (s, cc);
	}

	String_var (const std::basic_string <C>& s) :
		std::basic_string <C> (s)
	{}

	String_var (std::basic_string <C>&& s) :
		std::basic_string <C> (std::move (s))
	{}

	String_var& operator = (const C* s)
	{
		std::basic_string <C>::operator = (s);
		return *this;
	}

	template <class Tr, class Al>
	String_var& operator = (const std::basic_string <C, Tr, Al>& s)
	{
		std::basic_string <C>::operator = (s);
		return *this;
	}

	String_var& operator = (std::basic_string <C>&& s)
	{
		std::basic_string <C>::operator = (std::move (s));
		return *this;
	}

#ifdef LEGACY_STRING_MAPPING_SUPPORT

	// TODO: Mark as deprecated
	String_var& operator = (C* s)
	{
		this->release_memory ();
		adopt (s);
		return *this;
	}

#endif

	operator C* ()
	{
		return this->_ptr ();
	}

	operator const C* () const
	{
		return this->_ptr ();
	}

	typedef typename Type <StringT <C> >::C_in C_in;
	typedef typename Type <StringT <C> >::C_out C_out;
	typedef typename Type <StringT <C> >::C_inout C_inout;

	C_in in () const
	{
		return *this;
	}

	C_inout inout ()
	{
		return String_inout <C> (*this);
	}

	C_out out ()
	{
		return String_out <C> (*this);
	}

	StringT <C> _retn ()
	{
		return std::move (*this);
	}

private:

#ifdef LEGACY_STRING_MAPPING_SUPPORT
	void adopt (C* s);
#endif
};

#ifdef LEGACY_STRING_MAPPING_SUPPORT

template <typename C>
void String_var <C>::adopt (C* s)
{
	if (s) {
		size_t cc = std::char_traits <C>::length (s);
		this->large_pointer (s);
		this->large_size (cc);
		size_t au = ::Nirvana::StdString::memory ()->query (s, ::Nirvana::MemQuery::ALLOCATION_UNIT);
		this->large_allocated (::Nirvana::round_up ((cc + 1) * sizeof (C), au));
	} else
		this->reset ();
}

#endif

}

typedef Nirvana::String_var <Char> String_var;
typedef Nirvana::String_var <WChar> WString_var;

#ifdef LEGACY_STRING_MAPPING_SUPPORT

// For compatibility with old C++ mapping specification
Char* string_alloc (uint32_t len);
Char* string_dup (const Char* s);
void string_free (Char* s);
WChar* wstring_alloc (uint32_t len);
WChar* wstring_dup (const WChar* s);
void wstring_free (WChar* s);

#endif

}

#endif
