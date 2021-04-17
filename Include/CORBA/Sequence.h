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

#include <Nirvana/vector.h>
#include "TypeVarLen.h"

namespace CORBA {
namespace Nirvana {

template <typename T>
struct Type <Sequence <T> > :
	public TypeVarLen <Sequence <T>, CHECK_SEQUENCES || Type <T>::has_check>
{
	typedef TypeVarLen <Sequence <T>, CHECK_SEQUENCES || Type <T>::has_check> Base;
	typedef typename Type <T>::ABI_type T_ABI;
	typedef typename Base::Var_type Var_type;
	typedef typename Base::ABI_type ABI_type;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_inout ABI_inout;

	static void check (const ABI_type& v);

	class C_in : public Base::C_in
	{
	public:
		C_in (const Var_type& v) :
			Base::C_in (v)
		{}

		const ABI_type* operator & () const
		{
			// Use static_cast to ensure that we are using own vector implementation.
			return &static_cast <const ABI_type&> (this->ref_);
		}
	};

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Var_type& v) :
			Base::C_inout (v)
		{}

		ABI_type* operator & () const
		{
			// Use static_cast to ensure that we are using own vector implementation.
			return &static_cast <ABI_type&> (this->ref_);
		}
	};

	class C_out : public C_inout
	{
	public:
		C_out (Var_type& v) :
			C_inout (v)
		{
			v.clear ();
		}
	};

	static const Var_type& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <const Var_type&> (*p);
	}

	static Var_type& inout (ABI_inout p)
	{
		Base::inout (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <Var_type&> (*p);
	}

	static Var_type& out (ABI_out p)
	{
		Base::out (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		Var_type& val = static_cast <Var_type&> (*p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static TypeCode_ptr type_code ();

	static void marshal_in (const Var_type& src, Marshal_ptr marshaler, ABI_type& dst);
	static void marshal_out (Var_type& src, Marshal_ptr marshaler, ABI_type& dst);
	static void unmarshal (const ABI_type& src, Unmarshal_ptr unmarshaler, Var_type& dst);
};

template <typename T>
void Type <Sequence <T> >::check (const ABI_type& v)
{
	// Do some check
	if (CHECK_SEQUENCES) {
		const T* p = v.ptr;
		if (p)
			CORBA::Nirvana::_check_pointer (p);
		size_t cnt = v.size;
		if (cnt > 0 && (cnt > v.allocated / sizeof (T) || !Var_type::memory ()->is_readable (p, cnt * sizeof (T))))
			::Nirvana::throw_BAD_PARAM ();
	}

	if (Type <T>::has_check) {
		for (const T_ABI* p = v.ptr, *end = p + v.size; p != end; ++p)
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
	typedef typename Base::ABI_type ABI_type;
	typedef typename Base::ABI_ret ABI_ret;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_inout ABI_inout;
	typedef BoundedSequence <T, bound> Var_type;
	typedef BoundedSequence <T, bound> Member_type;

	static const bool has_check = true;

	static void check (const ABI_type& v)
	{
		if (Base::has_check)
			Base::check (v);
		if (v.size > bound)
			::Nirvana::throw_BAD_PARAM ();
	}

	typedef typename TypeVarLen <BoundedSequence <T, bound>, true, ABI_type>::C_ret C_ret;

	static const Var_type& in (ABI_in p)
	{
		return static_cast <const Var_type&> (Base::in (p));
	}

	static Var_type& out (ABI_out p)
	{
		return static_cast <Var_type&> (Base::out (p));
	}

	static Var_type& inout (ABI_inout p)
	{
		return static_cast <Var_type&> (Base::inout (p));
	}

	static TypeCode_ptr type_code ();
};

}
} // namespace CORBA

#endif
