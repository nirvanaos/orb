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

template <class I>
class TypeCodeValueAbstract :
	public TypeCodeStatic <TypeCodeValue <I>,
	TypeCodeWithId <Type <I>::tc_kind, I>, TypeCodeOps <I> >,
	public TypeCodeName <I>
{
	typedef TypeCodeStatic <TypeCodeValue <I>,
		TypeCodeWithId <Type <I>::tc_kind, I>, TypeCodeOps <I> > Base;
public:
	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (Base::RepositoryType::repository_id_,
			TypeCodeName <I>::name_, VM_ABSTRACT, nullptr, nullptr, 0);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equivalent (Base::RepositoryType::repository_id_,
			VM_ABSTRACT, nullptr, nullptr, 0);
	}

	static ULong _member_count (Bridge <TypeCode>* _b, Interface* _env) NIRVANA_NOEXCEPT
	{
		return 0;
	}

	static Type <String>::ABI_ret _member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) NIRVANA_NOEXCEPT
	{
		set_Bounds (_env);
		return Type <String>::ret ();
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) NIRVANA_NOEXCEPT
	{
		set_Bounds (_env);
		return nullptr;
	}

	static Visibility _member_visibility (Bridge <TypeCode>* _b, ULong index,
		Interface* _env) NIRVANA_NOEXCEPT
	{
		set_Bounds (_env);
		return 0;
	}

	static ValueModifier _type_modifier (Bridge <TypeCode>* _b, Interface* _env)
		NIRVANA_NOEXCEPT
	{
		return VM_ABSTRACT;
	}

	static Interface* _concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
		NIRVANA_NOEXCEPT
	{
		return nullptr;
	}
};

template <class T>
class TypeCodeStateMembers
{
public:
	static const StateMember* members () NIRVANA_NOEXCEPT
	{
		return members_;
	}

	static ULong member_count () NIRVANA_NOEXCEPT
	{
		return countof (members_);
	}

	static ULong _member_count (Bridge <TypeCode>* _b, Interface* _env)
	{
		return member_count ();
	}

	static Type <String>::ABI_ret _member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		if (index >= countof (members_)) {
			set_Bounds (_env);
			return Type <String>::ret ();
		} else
			return const_string_ret_p (members_ [index].name);
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		if (index >= countof (members_)) {
			set_Bounds (_env);
			return nullptr;
		} else {
			I_ptr <TypeCode> tc ((members_ [index].type) ());
			return (tc->_epv ().header.duplicate) (&tc, _env);
		}
	}

	static Visibility _member_visibility (Bridge <TypeCode>*, ULong index,
		Interface* _env)
	{
		if (index >= countof (members_)) {
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
	using TypeCodeMemberCount <0>::_member_count;

	static const StateMember* members () NIRVANA_NOEXCEPT
	{
		return nullptr;
	}

	static ULong member_count () NIRVANA_NOEXCEPT
	{
		return 0;
	}

	static Type <String>::ABI_ret _member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		set_Bounds (_env);
		return Type <String>::ret ();
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		set_Bounds (_env);
		return nullptr;
	}

	static Visibility _member_visibility (Bridge <TypeCode>*, ULong index,
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
	TypeCodeWithId <Type <I>::tc_kind, I>, TypeCodeOps <I> >,
	public TypeCodeName <I>,
	public TypeCodeStateMembersOptional <I, members>
{
	typedef TypeCodeStatic <TypeCodeValue <I>,
		TypeCodeWithId <Type <I>::tc_kind, I>, TypeCodeOps <I> > Base;
public:
	typedef TypeCodeName <I> Name;
	using Name::_name;

	typedef TypeCodeStateMembersOptional <I, members> Members;
	using Members::_member_count;
	using Members::_member_name;
	using Members::_member_type;
	using Members::_member_visibility;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (Base::_bridge (), Base::RepositoryType::repository_id_, Name::name_,
			vm, base, Members::members (), Members::member_count (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equivalent (Base::_bridge (), Base::RepositoryType::repository_id_,
			vm, base, Members::members (), Members::member_count (), other);
	}

	static ValueModifier _type_modifier (Bridge <TypeCode>* _b, Interface* _env)
	{
		return vm;
	}

	static Interface* _concrete_base_type (Bridge <TypeCode>* _b, Interface* _env)
	{
		if (!base)
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
