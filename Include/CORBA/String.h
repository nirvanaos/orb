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

#include "StringView.h"
#include "TypeVarLen.h"
#include <Nirvana/utf8.h>

namespace CORBA {

class TypeCode;

namespace Internal {

template <typename C>
struct Type <StringT <C> > : TypeVarLen <StringT <C> >
{
	typedef TypeVarLen <StringT <C> > Base;
	typedef typename Base::Var Var;
	typedef typename Base::ABI ABI;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;

	static void check (const ABI& s);

	typedef const StringView <C>& C_in;

	class C_out : public Base::C_inout
	{
	public:
		C_out (Var& s) :
			Base::C_inout (s)
		{
			s.clear ();
		}
	};

	// `const` is removed to let servant adopt the unmarshaled input data.
	static Var& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own basic_string implementation.
		return static_cast <StringT <C>&> (const_cast <ABI&> (*p));
	}

	static Var& inout (ABI_out p)
	{
		Base::inout (p); // Check
		return static_cast <Var&> (*p);
	}

	static Var& out (ABI_out p)
	{
		Base::out (p);	// Check
		Var& val = static_cast <Var&> (*p);
		// Must be empty
		if (!val.empty ())
			Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static I_ptr <TypeCode> type_code () noexcept;

	static void marshal_in (const Var& src, IORequest_ptr rq);
	static void marshal_out (Var& src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, Var& dst);

	inline
	static void check_encoding (const C* p, size_t cc);
};

template <> inline
void Type <StringT <Char> >::check_encoding (const Char* p, size_t cc)
{
	if (p [cc])
		Nirvana::throw_BAD_PARAM (); // Not zero-terminated

	if (!Nirvana::is_valid_utf8 (p, cc)) {
		assert (false);
		Nirvana::throw_CODESET_INCOMPATIBLE ();
	}
}

template <> inline
void Type <StringT <WChar> >::check_encoding (const WChar* p, size_t cc)
{
	if (p [cc])
		Nirvana::throw_BAD_PARAM (); // Not zero-terminated
}

template <typename C>
void Type <StringT <C> >::check (const ABI& s)
{
	// Do some check
	const C* p;
	size_t cc;
	if (s.is_large ()) {
		p = s.large_pointer ();
		cc = s.large_size ();
		check_pointer (p);
		if (cc > s.large_capacity ())
			::Nirvana::throw_BAD_PARAM ();
	} else {
		p = s.small_pointer ();
		cc = s.small_size ();
		if (cc > ABI::SMALL_CAPACITY)
			Nirvana::throw_BAD_PARAM ();
	}
	check_encoding (p, cc);
}

template <typename C, ULong bound>
class BoundedStringT : public StringT <C>
{
	typedef StringT <C> Base;
public:
	/// The String bound.
	static const ULong bound_ = bound;

	typedef typename Base::const_iterator const_iterator;

	// Implementations of the mapping are under no obligation to prevent assignment of a sequence to
	// a bounded sequence type if the sequence exceeds the bound. Implementations must at run time
	// detect attempts to pass a sequence that exceeds the bound as a parameter across an interface.

	// Constructors

	BoundedStringT ()
	{}

	BoundedStringT (const Base& s) :
		Base (s)
	{}

	BoundedStringT (Base&& s) noexcept :
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

	template <class InputIterator>
	BoundedStringT (InputIterator b, InputIterator e) :
		Base (b, e)
	{}

	BoundedStringT (const C* b, const C* e) :
		Base (b, e)
	{}

	BoundedStringT (const_iterator b, const_iterator e) :
		Base (b, e)
	{}

	BoundedStringT (std::initializer_list <C> ilist) :
		Base (ilist)
	{}

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
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;

	static void check (const ABI& v)
	{
		Base::check (v);
		if (v.size () > bound)
			Nirvana::throw_BAD_PARAM ();
	}

	// Check in C_in for member assignments
	typedef const StringView <C, bound>& C_in;

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Var& s) :
			Base::C_inout (s)
		{
			if (bound && s.size () > bound)
				Nirvana::throw_BAD_PARAM ();
		}
	};

	// `const` is removed to let servant adopt the unmarshaled input data.
	static Var& in (ABI_in p)
	{
		Var& v = Base::in (p);	// Check
		if (bound && v.size () > bound)
			Nirvana::throw_BAD_PARAM ();
		return v;
	}

	static Var& inout (ABI_out p)
	{
		Var& v = Base::inout (p); // Check
		if (bound && v.size () > bound)
			Nirvana::throw_BAD_PARAM ();
		return v;
	}

	static I_ptr <TypeCode> type_code () noexcept;

	static void unmarshal (IORequest_ptr rq, Var& dst)
	{
		Base::unmarshal (rq, dst);
		if (dst.size () > bound)
			Nirvana::throw_BAD_PARAM ();
	}
};

template <typename C, ULong bound>
template <class A, typename>
inline StringView <C, bound>::StringView (const std::basic_string <C, std::char_traits <C>, A>& s)
{
	size_t size = s.size ();
	if (bound && size > bound)
		Nirvana::throw_BAD_PARAM ();
	ABI::large_pointer (const_cast <C*> (s.data ()));
	ABI::large_size (size);
	ABI::allocated (0);
}

}
}

namespace IDL {

/// Bounded string type
/// 
/// \tparam bound Maximal string length.
template <uint32_t bound>
using BoundedString = CORBA::Internal::BoundedStringT <CORBA::Char, bound>;

/// Bounded wide string type
/// 
/// \tparam bound Maximal string length.
template <uint32_t bound>
using BoundedWString = CORBA::Internal::BoundedStringT <CORBA::WChar, bound>;

template <typename C>
struct traits <CORBA::Internal::StringT <C> > :
	CORBA::Internal::Traits <CORBA::Internal::StringT <C> >
{
	using element_traits = traits <C>;
	using is_bounded = std::false_type;
};

template <typename C, uint32_t _bound>
struct traits <CORBA::Internal::BoundedStringT <C, _bound> > :
	CORBA::Internal::Traits <CORBA::Internal::BoundedStringT <C, _bound> >
{
	using element_traits = traits <C>;
	using is_bounded = std::true_type;
	using bound = std::integral_constant <uint32_t, _bound>;
};

}

#endif
