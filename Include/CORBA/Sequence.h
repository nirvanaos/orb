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
#ifndef NIRVANA_ORB_SEQUENCE_H_
#define NIRVANA_ORB_SEQUENCE_H_
#pragma once

#include <Nirvana/vector.h>
#include "TypeVarLen.h"

namespace CORBA {
namespace Internal {

// Note that reference types are mapped to interface types.
// So Type <I_ref <I> > provide the same traits as Type <I>.

template <typename T>
struct Type <Sequence <T> > :
	public TypeVarLen <Sequence <T>, true>
{
	typedef TypeVarLen <Sequence <T>, true> Base;
	typedef typename Base::Var Var;
	typedef typename Base::ABI ABI;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;

	static void check (const ABI& v);
	static void check_ABI (const ABI& v);

	class C_in : public Base::C_in
	{
	public:
		C_in (const Var& v) :
			Base::C_in (v)
		{}

		const ABI* operator & () const
		{
			// Use static_cast to ensure that we are using own vector implementation.
			return &static_cast <const ABI&> (this->ref_);
		}
	};

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Var& v) :
			Base::C_inout (v)
		{}

		ABI* operator & () const
		{
			// Use static_cast to ensure that we are using own vector implementation.
			return &static_cast <ABI&> (this->ref_);
		}
	};

	class C_out : public C_inout
	{
	public:
		C_out (Var& v) :
			C_inout (v)
		{
			v.clear ();
		}
	};

	// `const` is removed to let servant adopt the unmarshaled input data.
	static Var& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <Var&> (const_cast <ABI&> (*p));
	}

	static Var& inout (ABI_out p)
	{
		Base::inout (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <Var&> (*p);
	}

	static Var& out (ABI_out p)
	{
		Base::out (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		Var& val = static_cast <Var&> (*p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static I_ptr <TypeCode> type_code () NIRVANA_NOEXCEPT;

	static void marshal_in (const Var& src, IORequest_ptr rq);
	static void marshal_out (Var& src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, Var& dst);
};

template <typename T>
void Type <Sequence <T> >::check_ABI (const ABI& v)
{
	// Do some check
	if (v.size > v.allocated / sizeof (T))
		::Nirvana::throw_BAD_PARAM ();
	if (v.allocated)
		check_pointer (v.ptr);
}

template <typename T>
void Type <Sequence <T> >::check (const ABI& v)
{
	check_ABI (v);

	typedef typename Type <T>::ABI T_ABI;
	if (Type <T>::has_check) {
		for (const T_ABI* p = (const T_ABI*)v.ptr, *end = p + v.size; p != end; ++p)
			Type <T>::check (*p);
	}
}

/// Bounded sequence type.
/// 
/// \tparam T Element type.
/// \tparam bound The bound.
template <typename T, ULong bound>
class BoundedSequence : public Sequence <T>
{
	typedef Sequence <T> Base;
public:
	/// The sequence bound.
	static const ULong bound_ = bound;

	// Implementations of the mapping are
	// under no obligation to prevent assignment of a sequence to a bounded sequence type if the sequence exceeds the bound.
	// Implementations must at run time detect attempts to pass a sequence that exceeds the bound as a parameter across an
	// interface.

	// Constructors

	BoundedSequence ()
	{}

	explicit BoundedSequence (size_t count) :
		Base (count)
	{}

#ifndef NIRVANA_C11
	explicit
#endif
	BoundedSequence (size_t count, const T& v) :
		Base (count, v)
	{}

	BoundedSequence (const Base& src) :
		Base (src)
	{}

	BoundedSequence (Base&& src) NIRVANA_NOEXCEPT :
		Base (std::move (src))
	{}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	BoundedSequence (InputIterator b, InputIterator e) :
		Base (b, e)
	{}

#ifdef NIRVANA_C11
	BoundedSequence (std::initializer_list <T> ilist) :
		Base (ilist)
	{}
#endif
};

template <class T, ULong bound>
struct Type <BoundedSequence <T, bound> > :
	public Type <Sequence <T> >
{
	typedef Type <Sequence <T> > Base;
	typedef typename Base::ABI ABI;
	typedef typename Base::Var Var;

	static const bool has_check = true;

	static void check (const ABI& v)
	{
		if (Base::has_check)
			Base::check (v);
		if (v.size > bound)
			Nirvana::throw_BAD_PARAM ();
	}

	static I_ptr <TypeCode> type_code () NIRVANA_NOEXCEPT;

	static void unmarshal (IORequest_ptr rq, Var& dst)
	{
		Base::unmarshal (rq, dst);
		if (dst.size () > bound)
			Nirvana::throw_BAD_PARAM ();
	}
};

}
} // namespace CORBA

#endif
