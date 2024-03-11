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
#ifndef NIRVANA_ORB_TYPECODEARRAY_H_
#define NIRVANA_ORB_TYPECODEARRAY_H_

#include "TypeCodeImpl.h"
#include "Type_array.h"
#include "ORB.h"

namespace CORBA {
namespace Internal {

template <class T, size_t bound>
class TypeCodeArray :
	public TypeCodeStatic <TypeCodeArray <T, bound>, TypeCodeTK <TCKind::tk_array>,
		TypeCodeOps <std::array <T, bound> > >,
	public TypeCodeLength <bound>,
	public TypeCodeContentType <T>
{
	typedef TypeCodeStatic <TypeCodeArray <T, bound>, TypeCodeTK <TCKind::tk_array>,
		TypeCodeOps <std::array <T, bound> > > Base;
	typedef TypeCodeContentType <T> ContentType;
public:
	using TypeCodeLength <bound>::_s_length;
	using ContentType::_s_content_type;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equal (TCKind::tk_array, bound, ContentType::ptr (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equivalent (TCKind::tk_array, bound, ContentType::ptr (), other);
	}

	static Type <CORBA::TypeCode>::VRet get_compact_typecode ()
	{
		I_ptr <TypeCode> content = ContentType::ptr ();
		I_ref <TypeCode> compact = content->get_compact_typecode ();
		if (&content == &I_ptr <TypeCode> (compact))
			return I_ptr <TypeCode> (Base::_get_ptr ());
		else
			return orb_impl->create_array_tc (bound, compact);
	}
};

template <typename T, size_t bound> inline
I_ptr <TypeCode> Type <std::array <T, bound> >::type_code () noexcept
{
	return TypeCodeArray <T, bound>::_get_ptr ();
}

}
}

#endif
