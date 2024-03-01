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
#include "../../pch/pch.h"
#include <CORBA/TypeCodeString.h>
#include <CORBA/TypeCodeValue.h>
#include <CORBA/TypeCodeInterface.h>

namespace CORBA {
namespace Internal {

template <typename T, TCKind tk>
class TypeCodeScalar : public TypeCodeStatic <TypeCodeScalar <T, tk>,
	TypeCodeTK <tk>, TypeCodeOps <T> > {};

template <>
class TypeCodeValue <ValueBase> : public TypeCodeValueConcrete <ValueBase, VM_NONE, false, nullptr>
{};

template <>
const Char TypeCodeName <ValueBase>::name_ [] = "ValueBase";

class TC_TypeCode : public TypeCodeStatic <TC_TypeCode,
	TypeCodeTK <TCKind::tk_TypeCode>, TypeCodeOps <TypeCode> >
{};

}

#define TC_IMPL(t, ...) extern NIRVANA_SELECTANY const ::CORBA::Internal::StaticTC _tc_##t { &Internal::__VA_ARGS__::epv_ }

#define TC_IMPL_SCALAR(T, t) TC_IMPL (t, TypeCodeScalar <T, TCKind::tk_##t>)

TC_IMPL_SCALAR (void, void);
TC_IMPL_SCALAR (Short, short);
TC_IMPL_SCALAR (Long, long);
TC_IMPL_SCALAR (UShort, ushort);
TC_IMPL_SCALAR (ULong, ulong);
TC_IMPL_SCALAR (Float, float);
TC_IMPL_SCALAR (Double, double);
TC_IMPL_SCALAR (Boolean, boolean);
TC_IMPL_SCALAR (Char, char);
TC_IMPL_SCALAR (Octet, octet);
TC_IMPL_SCALAR (LongLong, longlong);
TC_IMPL_SCALAR (ULongLong, ulonglong);
TC_IMPL_SCALAR (LongDouble, longdouble);
TC_IMPL_SCALAR (WChar, wchar);
TC_IMPL_SCALAR (Any, any);

TC_IMPL (string, TypeCodeString <Internal::String, 0>);
TC_IMPL (wstring, TypeCodeString <Internal::WString, 0>);

NIRVANA_STATIC_TC (_tc_Object, TypeCodeInterface <Object>, Internal::RepIdOf <Object>::id, "Object");

TC_IMPL (ValueBase, TypeCodeValue <ValueBase>);

TC_IMPL (TypeCode, TC_TypeCode);

}
