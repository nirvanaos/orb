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
#pragma once

#include <Nirvana/basic_string.h>
#include "TypeVarLen.h"

namespace CORBA {
namespace Internal {

template <typename C, ULong bound>
template <class A> inline
StringBase <C, bound>::StringBase (const std::basic_string <C, std::char_traits <C>, A>& s)
{
	size_t size = s.size ();
	if (bound && size > bound)
		Nirvana::throw_BAD_PARAM ();
	this->large_pointer (const_cast <C*> (s.data ()));
	this->large_size (size);
	this->allocated (0);
}

template <typename C>
struct Type <StringT <C> > : TypeVarLen <StringT <C>, CHECK_STRINGS>
{
	typedef TypeVarLen <StringT <C>, CHECK_STRINGS> Base;
	typedef typename Base::Var Var;
	typedef typename Base::ABI ABI;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;

	static void check (const ABI& s);

	typedef const StringBase <C>& C_in;

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Var& s) :
			Base::C_inout (s)
		{}

		ABI* operator & () const
		{
			return &static_cast <ABI&> (this->ref_);
		}
	};

	class C_out : public C_inout
	{
	public:
		C_out (Var& s) :
			C_inout (s)
		{
			s.clear ();
		}
	};

	static const Var& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		return static_cast <const StringT <C>&> (*p);
	}

	static Var& inout (ABI_out p)
	{
		Base::inout (p); // Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		return static_cast <Var&> (*p);
	}

	static Var& out (ABI_out p)
	{
		Base::out (p);	// Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		Var& val = static_cast <Var&> (*p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static I_ptr <TypeCode> type_code () NIRVANA_NOEXCEPT;

	static bool _small_copy (const ABI& src, ABI& dst);

	static void marshal_in (const Var& src, IORequest_ptr rq);
	static void marshal_out (Var& src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, Var& dst);
};

template <typename C>
void Type <StringT <C> >::check (const ABI& s)
{
	// Do some check
	const C* p;
	size_t cc;
	if (s.is_large ()) {
		p = s.large_pointer ();
		cc = s.large_size ();
		CORBA::Internal::_check_pointer (p);
		if (cc > s.large_capacity ())
			::Nirvana::throw_BAD_PARAM ();
	} else {
		p = s.small_pointer ();
		cc = s.small_size ();
		if (cc > ABI::SMALL_CAPACITY)
			::Nirvana::throw_BAD_PARAM ();
	}
	if (p [cc])
		::Nirvana::throw_BAD_PARAM (); // Not zero-terminated
}

template <typename C>
bool Type <StringT <C> >::_small_copy (const ABI& src, ABI& dst)
{
	const C* p;
	size_t len;
	if (src.is_large ()) {
		len = src.large_size ();
		if (len > ABI::SMALL_CAPACITY)
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

template <typename C, ULong bound>
class BoundedStringT : public StringT <C>
{
	typedef StringT <C> Base;
public:
	/// The String bound.
	static const ULong bound_ = bound;

	typedef typename Base::const_iterator const_iterator;

	// Implementations of the mapping are
	// under no obligation to prevent assignment of a sequence to a bounded sequence type if the sequence exceeds the bound.
	// Implementations must at run time detect attempts to pass a sequence that exceeds the bound as a parameter across an
	// interface.

	// Constructors

	BoundedStringT ()
	{}

	BoundedStringT (const Base& s) :
		Base (s)
	{}

	BoundedStringT (Base&& s) NIRVANA_NOEXCEPT :
		Base (std::move (s))
	{}

	BoundedStringT (const Base& src, size_t off, size_t cnt = Base::npos) :
		Base (src, off, cnt)
	{}

	BoundedStringT (const C* ptr, size_t cnt) :
		Base (ptr, cnt)
	{}

	BoundedStringT (const C* ptr) :
		Base (ptr)
	{}

	BoundedStringT (size_t cnt, C c) :
		Base (cnt, c)
	{}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	BoundedStringT (InputIterator b, InputIterator e) :
		Base (b, e)
	{}

	BoundedStringT (const C* b, const C* e) :
		Base (b, e)
	{}

	BoundedStringT (const_iterator b, const_iterator e) :
		Base (b, e)
	{}

#ifdef NIRVANA_C11

	BoundedStringT (std::initializer_list <C> ilist) :
		Base (ilist)
	{}

#endif

#ifdef NIRVANA_C17
	typedef std::basic_string_view <C, std::char_traits <C> > __sv_type;

	template<typename _Tp, typename _Res>
	using _If_sv = std::enable_if_t <
		std::is_convertible <const _Tp&, __sv_type>::value
		&& !std::is_convertible <const _Tp*, const Base*>::value
		&& !std::is_convertible <const _Tp&, const C*>::value,
		_Res>;

	template <class V, class = _If_sv <V, void> >
	BoundedStringT (const V& v) :
		Base (v)
	{}

#endif
};

template <typename C, ULong bound>
struct Type <BoundedStringT <C, bound> > : Type <StringT <C> >
{
	typedef Type <StringT <C> > Base;
	typedef typename Base::ABI ABI;
	typedef typename Base::Var Var;

	static const bool has_check = true;

	static void check (const ABI& v)
	{
		Base::check (v);
		if (v.size > bound)
			::Nirvana::throw_BAD_PARAM ();
	}

	// Check in C_in for member assignments
	typedef const StringBase <C, bound>& C_in;

	static I_ptr <TypeCode> type_code () NIRVANA_NOEXCEPT;
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

}
}

#endif
