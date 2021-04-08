/// \file
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
#ifndef NIRVANA_ORB_STRING_H_
#define NIRVANA_ORB_STRING_H_

#include <Nirvana/basic_string.h>
#include "TypeVarLen.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
struct Type <StringT <C> > : TypeVarLen <StringT <C>, CHECK_STRINGS>
{
	typedef TypeVarLen <StringT <C>, CHECK_STRINGS> Base;
	typedef typename Base::ABI_type ABI_type;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_inout ABI_inout;
	typedef typename Base::Member_type Member_type;

	static void check (const ABI_type& s);

	typedef const StringBase <C>& C_in;

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (StringT <C>& s) :
			Base::C_inout (s)
		{}

		ABI_type* operator & () const
		{
			return &static_cast <ABI_type&> (this->ref_);
		}
	};

	class C_out : public C_inout
	{
	public:
		C_out (StringT <C>& s) :
			C_inout (s)
		{
			s.clear ();
		}
	};

	static const StringT <C>& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		return static_cast <const StringT <C>&> (*p);
	}

	static StringT <C>& inout (ABI_inout p)
	{
		Base::inout (p); // Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		return static_cast <StringT <C>&> (*p);
	}

	static StringT <C>& out (ABI_out p)
	{
		Base::out (p);	// Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		StringT <C>& val = static_cast <StringT <C>&> (*p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static TypeCode_ptr type_code ();
};

template <typename C>
void Type <StringT <C> >::check (const ABI_type& s)
{
	// Do some check
	const C* p;
	size_t cc;
	if (s.is_large ()) {
		p = s.large_pointer ();
		cc = s.large_size ();
		CORBA::Nirvana::_check_pointer (p);
		if (cc > s.large_capacity () || !StringT <C>::memory ()->is_readable (p, (cc + 1) * sizeof (C)))
			::Nirvana::throw_BAD_PARAM ();
	} else {
		p = s.small_pointer ();
		cc = s.small_size ();
		if (cc > ABI_type::SMALL_CAPACITY)
			::Nirvana::throw_BAD_PARAM ();
	}
	if (p [cc])
		::Nirvana::throw_BAD_PARAM (); // Not zero-terminated
}

template <typename C, ULong bound = 0>
class BoundedStringT : public StringT <C>
{
public:
	static const ULong bound_ = bound;
};

template <typename C, ULong bound>
struct Type <BoundedStringT <C, bound> > : Type <StringT <C> >
{
	typedef Type <StringT <C> > Base;
	typedef typename Base::ABI_type ABI_type;
	typedef typename Base::Var_type Var_type;

	static const bool has_check = Base::has_check || bound != 0;

	static void check (const ABI_type& v)
	{
		Base::check (v);
		if (bound && v.size > bound)
			::Nirvana::throw_BAD_PARAM ();
	}

	// Check in C_in for member assignments
	class C_in : public Base::C_in
	{
	public:
		C_in (const Var_type& s) :
			Base::C_in (s)
		{
			if (bound && s.size () > bound)
				::Nirvana::throw_BAD_PARAM ();
		}
	};

	static TypeCode_ptr type_code ();
};

/// Unbounded string. Equivalent to std::string.
typedef StringT <Char> String;

/// Unbounded wide string. Equivalent to std::wstring.
typedef StringT <WChar> WString;

/// Bounded string type
/// 
/// \tparam bound Maximal string length.
template <ULong bound>
using BoundedString = BoundedStringT <Char, bound>;

/// Bounded wide string type
/// 
/// \tparam bound Maximal string length.
template <ULong bound>
using BoundedWString = BoundedStringT <WChar, bound>;

// For member assignments
template <typename C> inline
StringBase <C>::operator const StringT <C>& () const
{
	return static_cast <const StringT <C>&> (*this);
}

}

typedef typename Nirvana::Type <Nirvana::StringT <Char> >::C_out String_out;
typedef typename Nirvana::Type <Nirvana::StringT <WChar> >::C_out WString_out;
typedef typename Nirvana::Type <Nirvana::StringT <Char> >::C_inout String_inout;
typedef typename Nirvana::Type <Nirvana::StringT <WChar> >::C_inout WString_inout;

// For String_in and WString_in see StringBase.h
// String_var and WString_var classes are defined in String_compat.h

}

#endif
