/// \file TypeCodeString.h
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#include "../String.h"

namespace CORBA {
namespace Nirvana {

template <typename Valtype, ULong bound> class TypeCodeString;

template <typename Valtype, TCKind tk, ULong bound>
class TypeCodeStringBase :
	public TypeCodeStatic <TypeCodeString <Valtype, bound>, TypeCodeTK <tk>, TypeCodeOps <Valtype> >,
	public TypeCodeLength <bound>
{
public:
	using TypeCodeLength <bound>::_length;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeTK <tk>::equal (other)
			&& other->length () == bound;
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeTK <tk>::equivalent (other)
			&& other->length () == bound;
	}
};

template <ULong bound>
class TypeCodeString <String, bound> : public TypeCodeStringBase <String, tk_string, bound> {};

template <ULong bound>
class TypeCodeString <WString, bound> : public TypeCodeStringBase <WString, tk_wstring, bound> {};

}
}

#endif
