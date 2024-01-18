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
#ifndef NIRVANA_ORB_TYPECODEVALUE_H_
#define NIRVANA_ORB_TYPECODEVALUE_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Internal {

template <class I> class TypeCodeValue;

template <class T>
class TypeCodeStateMembers
{
public:
	static const StateMember* members () noexcept
	{
		return members_;
	}

	static ULong member_count () noexcept
	{
		return (ULong)countof (members_);
	}

	static ULong _s_member_count (Bridge <TypeCode>* _b, Interface* _env)
	{
		return member_count ();
	}

	static Type <String>::ABI_ret _s_member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		if (index >= member_count ()) {
			set_Bounds (_env);
			return Type <String>::ret ();
		} else
			return const_string_ret_p (members_ [index].name);
	}

	static Interface* _s_member_type (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		if (index >= member_count ()) {
			set_Bounds (_env);
			return nullptr;
		} else {
			I_ptr <TypeCode> tc ((members_ [index].type) ());
			return (tc->_epv ().header.duplicate) (&tc, _env);
		}
	}

	static Visibility _s_member_visibility (Bridge <TypeCode>*, ULong index,
		Interface* _env)
	{
		if (index >= member_count ()) {
			set_Bounds (_env);
			return 0;
		} else
			return members_ [index].visibility;
	}

protected:
	static const StateMember members_ [];
};

class TypeCodeStateMembersEmpty :
	public TypeCodeMemberCount <0>
{
public:
	using TypeCodeMemberCount <0>::_s_member_count;

	static const StateMember* members () noexcept
	{
		return nullptr;
	}

	static ULong member_count () noexcept
	{
		return 0;
	}

	static Type <String>::ABI_ret _s_member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		set_Bounds (_env);
		return Type <String>::ret ();
	}

	static Interface* _s_member_type (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		set_Bounds (_env);
		return nullptr;
	}

	static Visibility _s_member_visibility (Bridge <TypeCode>*, ULong index,
		Interface* _env)
	{
		set_Bounds (_env);
		return 0;
	}
};

template <class T, bool members>
using TypeCodeStateMembersOptional = typename std::conditional <members,
	TypeCodeStateMembers <T>, TypeCodeStateMembersEmpty>::type;

template <class I, ValueModifier vm, bool members, GetTypeCode base = nullptr>
class TypeCodeValueConcrete :
	public TypeCodeStatic <TypeCodeValue <I>,
	TypeCodeWithId <TCKind::tk_value, I>, TypeCodeOps <I> >,
	public TypeCodeName <I>,
	public TypeCodeStateMembersOptional <I, members>
{
	typedef TypeCodeStatic <TypeCodeValue <I>,
		TypeCodeWithId <TCKind::tk_value, I>, TypeCodeOps <I> > Base;
public:
	typedef TypeCodeName <I> Name;
	using Name::_s_name;

	typedef TypeCodeStateMembersOptional <I, members> Members;
	using Members::_s_member_count;
	using Members::_s_member_name;
	using Members::_s_member_type;
	using Members::_s_member_visibility;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return g_ORB->tc_equal (Base::_get_ptr (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return g_ORB->tc_equivalent (Base::_get_ptr (), other);
	}

	static Type <CORBA::TypeCode>::VRet get_compact_typecode ()
	{
		return g_ORB->get_compact_typecode (Base::_get_ptr ());
	}

	static ValueModifier _s_type_modifier (Bridge <TypeCode>* _b, Interface* _env)
	{
		return vm;
	}

	static Interface* _s_concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		if (base == nullptr)
			return nullptr;
		else {
			I_ptr <TypeCode> tc ((base)());
			return (tc->_epv ().header.duplicate) (&tc, _env);
		}
	}

};

}
}

#endif
