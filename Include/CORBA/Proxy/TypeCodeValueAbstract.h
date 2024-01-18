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
#ifndef NIRVANA_ORB_TYPECODEVALUEABSTRACT_H_
#define NIRVANA_ORB_TYPECODEVALUEABSTRACT_H_

#include "../TypeCodeImpl.h"

namespace CORBA {
namespace Internal {

template <class I>
class TypeCodeValueAbstract :
	public TypeCodeStatic <TypeCodeValueAbstract <I>,
		TypeCodeWithId <TCKind::tk_value, I>, TypeCodeOps <I> >,
	public TypeCodeName <I>,
	public TypeCodeORB
{
	typedef TypeCodeStatic <TypeCodeValueAbstract <I>,
		TypeCodeWithId <TCKind::tk_value, I>, TypeCodeOps <I> > Base;
public:
	typedef TypeCodeName <I> Name;
	using Name::_s_name;
	using TypeCodeORB::_s_equal;
	using TypeCodeORB::_s_equivalent;
	using TypeCodeORB::_s_get_compact_typecode;

	static ULong _s_member_count (Bridge <TypeCode>* _b, Interface* _env) noexcept
	{
		return 0;
	}

	static Type <String>::ABI_ret _s_member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) noexcept
	{
		set_Bounds (_env);
		return Type <String>::ret ();
	}

	static Interface* _s_member_type (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) noexcept
	{
		set_Bounds (_env);
		return nullptr;
	}

	static Visibility _s_member_visibility (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) noexcept
	{
		set_Bounds (_env);
		return 0;
	}

	static ValueModifier _s_type_modifier (Bridge <TypeCode>* _b, Interface* _env)
		noexcept
	{
		return VM_ABSTRACT;
	}

	static Interface* _s_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
		noexcept
	{
		return nullptr;
	}
};

}
}

#endif
