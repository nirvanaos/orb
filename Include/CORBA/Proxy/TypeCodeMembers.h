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
#ifndef NIRVANA_ORB_TYPECODEMEMBERS_H_
#define NIRVANA_ORB_TYPECODEMEMBERS_H_

#include "../TypeCodeImpl.h"
#include "InterfaceMetadata.h"
#include <type_traits>

namespace CORBA {
namespace Internal {

template <class T>
class TypeCodeMembers
{
public:
	static const Parameter* members () noexcept
	{
		return members_;
	}

	static ULong member_count () noexcept
	{
		return (ULong)countof (members_);
	}

	static ULong _s_member_count (Bridge <TypeCode>* _b, Interface* _env) noexcept
	{
		return member_count ();
	}

	static Type <String>::ABI_ret _s_member_name (Bridge <TypeCode>*_b, ULong index,
		Interface * _env)
	{
		if (index >= countof (members_)) {
			set_Bounds (_env);
			return Type <String>::ret ();
		} else
			return const_string_ret_p (members_ [index].name);
	}

	static Interface* _s_member_type (Bridge <TypeCode>*_b, ULong index,
		Interface * _env)
	{
		if (index >= countof (members_)) {
			set_Bounds (_env);
			return nullptr;
		} else {
			I_ptr <TypeCode> tc ((members_ [index].type) ());
			return (tc->_epv ().header.duplicate) (&tc, _env);
		}
	}

protected:
	static const Parameter members_ [];
};

class TypeCodeMembersEmpty :
	public TypeCodeMemberCount <0>
{
public:
	using TypeCodeMemberCount <0>::_s_member_count;

	static const Parameter* members () noexcept
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
};

template <class T, bool members>
using TypeCodeMembersOptional = typename std::conditional <members, 
	TypeCodeMembers <T>, TypeCodeMembersEmpty>::type;

}
}

#endif
