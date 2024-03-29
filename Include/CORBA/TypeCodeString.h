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
#ifndef NIRVANA_ORB_TYPECODESTRING_H_
#define NIRVANA_ORB_TYPECODESTRING_H_

#include "TypeCodeImpl.h"
#include "String.inl"

namespace CORBA {
namespace Internal {

template <typename Valtype, ULong bound> class TypeCodeString;

template <typename Valtype, TCKind tk, ULong bound>
class TypeCodeStringBase :
	public TypeCodeStatic <TypeCodeString <Valtype, bound>, TypeCodeTK <tk>,
		TypeCodeOps <Valtype> >,
	public TypeCodeLength <bound>
{
	typedef TypeCodeStatic <TypeCodeString <Valtype, bound>, TypeCodeTK <tk>,
		TypeCodeOps <Valtype> > Base;

public:
	using TypeCodeLength <bound>::_s_length;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equal (tk, bound, other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equivalent (tk, bound, other);
	}
};

template <ULong bound>
class TypeCodeString <StringT <Char>, bound> :
	public TypeCodeStringBase <String, TCKind::tk_string, bound> {};

template <ULong bound>
class TypeCodeString <StringT <WChar>, bound> :
	public TypeCodeStringBase <WString, TCKind::tk_wstring, bound> {};

template <typename C, ULong bound> inline
I_ptr <TypeCode> Type <BoundedStringT <C, bound> >::type_code () noexcept
{
	if (!bound)
		return Type <StringT <C> >::type_code ();
	else
		return TypeCodeString <StringT <C>, bound>::_get_ptr ();
}

}
}

#endif
