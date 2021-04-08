/// \file
/// CORBA sequence declarations.

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

template <class T, ULong b = 0>
class Sequence : public std::vector <T>
{
public:
	static const ULong bound = b;
};

template <class T, ULong bound>
struct Type <Sequence <T, bound> > :
	public TypeVarLen <std::vector <T>, CHECK_SEQUENCES || bound || Type <T>::has_check>
{
	typedef std::vector <T> Container;
	typedef TypeVarLen <Container, CHECK_SEQUENCES || bound || Type <T>::has_check> Base;
	typedef typename Type <T>::ABI_type T_ABI;
	typedef ABI <std::vector <T_ABI> > ABI_type;

	static void check (const ABI_type& v);

	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_inout ABI_inout;

	class C_in : public Base::C_in
	{
	public:
		C_in (const Container& s) :
			Base::C_in (s)
		{
			if (bound && s.size () > bound)
				::Nirvana::throw_BAD_PARAM ();
		}

		const ABI_type* operator & () const
		{
			return &static_cast <const ABI_type&> (this->ref_);
		}
	};

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Container& s) :
			Base::C_inout (s)
		{
			if (bound && s.size () > bound)
				::Nirvana::throw_BAD_PARAM ();
		}

		ABI_type* operator & () const
		{
			return &static_cast <ABI_type&> (this->ref_);
		}
	};

	class C_out : public C_inout
	{
	public:
		C_out (Container& s) :
			C_inout (s)
		{
			s.clear ();
		}
	};

	static const Container& in (ABI_in p)
	{
		Base::in (p);	// Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <const Sequence <T>&> (*p);
	}

	static Container& inout (ABI_inout p)
	{
		Base::inout (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		return static_cast <Container&> (*p);
	}

	static Container& out (ABI_out p)
	{
		Base::out (p); // Check
		// Use static_cast to ensure that we are using own vector implementation.
		Container& val = static_cast <Container&> (*p);
		// Must be empty
		if (!val.empty ())
			::Nirvana::throw_BAD_PARAM ();
		return val;
	}

	static TypeCode_ptr type_code ();
};

template <class T>
struct Type < ::std::vector <T> > : public Type <Sequence <T, 0> > {};

template <class T, ULong bound>
void Type <Sequence <T, bound> >::check (const ABI_type& v)
{
	// Do some check
	if (CHECK_SEQUENCES) {
		const T* p = v.ptr;
		if (p)
			CORBA::Nirvana::_check_pointer (p);
		size_t cnt = v.size;
		if (cnt > 0 && (cnt > v.allocated / sizeof (T) || !Container::memory ()->is_readable (p, cnt * sizeof (T))))
			::Nirvana::throw_BAD_PARAM ();
	}

	if (bound && v.size > bound)
		::Nirvana::throw_BAD_PARAM ();

	if (Type <T>::has_check) {
		for (const T_ABI* p = v.ptr, *end = p + v.size; p != end; ++p)
			Type <T>::check (*p);
	}
}

}
} // namespace CORBA

#endif
