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
#include "../sequence.h"

namespace CORBA {
namespace Nirvana {

template <typename T, class Content, ULong bound = 0>
class TypeCodeSequence :
	public TypeCodeStatic <TypeCodeSequence <T, Content, bound>, TypeCodeTK <tk_sequence>, TypeCodeOps <Sequence <T> > >,
	public TypeCodeLength <bound>,
	public TypeCodeContentType <Content>
{
public:
	using TypeCodeLength <bound>::_length;
	using TypeCodeContentType <Content>::_content_type;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeTK <tk_sequence>::equal (other)
			&& other->length () == bound
			&& other->content_type ()->equal (TypeCodeContentType <Content>::ptr ());
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeTK <tk_sequence>::equivalent (other)
			&& other->length () == bound
			&& other->content_type ()->equivalent (TypeCodeContentType <Content>::ptr ());
	}
};

}
}

#endif
