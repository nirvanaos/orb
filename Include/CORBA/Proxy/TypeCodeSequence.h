/// \file TypeCodeSequence.h
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
#ifndef NIRVANA_ORB_TYPECODESEQUENCE_H_
#define NIRVANA_ORB_TYPECODESEQUENCE_H_

#include "TypeCodeImpl.h"
#include "../Sequence.h"

namespace CORBA {
namespace Internal {

template <typename T, ULong bound = 0>
class TypeCodeSequence :
	public TypeCodeStatic <TypeCodeSequence <T, bound>, TypeCodeTK <tk_sequence>, TypeCodeOps <Sequence <T> > >,
	public TypeCodeLength <bound>,
	public TypeCodeContentType <T>
{
	typedef TypeCodeContentType <T> ContentType;
public:
	using TypeCodeLength <bound>::_length;
	using TypeCodeContentType <T>::_content_type;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeTK <tk_sequence>::equal (other)
			&& other->length () == bound
			&& other->content_type ()->equal (ContentType::ptr ());
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		I_ref <TypeCode> otc = TypeCodeBase::dereference_alias (other);
		return TypeCodeBase::equivalent (tk_sequence, otc)
			&& otc->length () == bound
			&& otc->content_type ()->equivalent (ContentType::ptr ());
	}
};

template <typename T> inline
I_ptr <TypeCode> Type <Sequence <T> >::type_code ()
{
	return TypeCodeSequence <T, 0>::_get_ptr ();
}

template <typename T, ULong bound> inline
I_ptr <TypeCode> Type <BoundedSequence <T, bound> >::type_code ()
{
	return TypeCodeSequence <T, bound>::_get_ptr ();
}

}
}

#endif
