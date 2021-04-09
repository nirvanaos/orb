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
	typedef typename Base::Var_type Var_type;
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
		C_inout (Var_type& s) :
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
		C_out (Var_type& s) :
			C_inout (s)
		{
			s.clear ();
		}
	};

	static const Var_type& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		return static_cast <const StringT <C>&> (*p);
	}

	static Var_type& inout (ABI_inout p)
	{
		Base::inout (p); // Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		return static_cast <Var_type&> (*p);
	}

	static Var_type& out (ABI_out p)
	{
		Base::out (p);	// Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		Var_type& val = static_cast <Var_type&> (*p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static TypeCode_ptr type_code ();

	static bool _small_copy (const ABI_type& src, ABI_type& dst);

	static void marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst);

	static void marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst);

	static void unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Var_type& dst)
	{
		if (Base::has_check)
			check (src);

		if (src.is_large ()) {
			size_t cb = src.allocated ();
			if (cb) {
				unmarshaler->adopt_memory (src.large_pointer (), cb);
				static_cast <ABI_type&> (dst) = src;
			} else
				dst.assign_internal (src.large_size (), src.large_pointer ());
		} else
			static_cast <ABI_type&> (dst) = src;
	}
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
		if (cc > s.large_capacity () || !Var_type::memory ()->is_readable (p, (cc + 1) * sizeof (C)))
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

template <typename C>
void Type <StringT <C> >::marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = Var_type::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (const_cast <C*> (src.large_pointer ()), cb, 0));
		dst.large_size (size);
		dst.allocated (cb);
	}
}

template <typename C>
void Type <StringT <C> >::marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst)
{
	assert (&src != &dst);
	if (!_small_copy (src, dst)) {
		size_t size = src.large_size ();
		size_t cb = Var_type::byte_size (size);
		dst.large_pointer ((C*)marshaler->marshal_memory (src.large_pointer (), cb, src.allocated ()));
		dst.large_size (size);
		dst.allocated (cb);
		src.reset ();
	}
}

template <typename C>
bool Type <StringT <C> >::_small_copy (const ABI_type& src, ABI_type& dst)
{
	const C* p;
	size_t len;
	if (src.is_large ()) {
		len = src.large_size ();
		if (len > ABI_type::SMALL_CAPACITY)
			return false;
		p = src.large_pointer ();
	} else {
		p = src.small_pointer ();
		len = src.small_size ();
	}
	::Nirvana::real_copy (p, p + len + 1, dst.small_pointer ());
	dst.small_size (len);
	return true;
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
