/// \file TypeCodeEnum.h
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
#ifndef NIRVANA_ORB_TYPECODEENUM_H_
#define NIRVANA_ORB_TYPECODEENUM_H_

#include "../TypeCodeImpl.h"
#include "../TypeEnum.h"

namespace CORBA {
namespace Internal {

template <class E>
class TypeCodeEnum :
	public TypeCodeStatic <TypeCodeEnum <E>, TypeCodeWithId <TCKind::tk_enum, E>,
		TypeCodeOps <E> >,
	public TypeCodeMemberCount <Type <E>::count_>,
	public TypeCodeName <E>
{
	typedef TypeCodeStatic <TypeCodeEnum <E>, TypeCodeWithId <TCKind::tk_enum, E>,
		TypeCodeOps <E> > Base;

	typedef ServantStatic <TypeCodeEnum <E>, TypeCode> Servant;

public:
	using TypeCodeMemberCount <Type <E>::count_>::_s_member_count;
	using TypeCodeName <E>::_s_name;
	using Servant::_s_get_compact_typecode;

	static Type <String>::ABI_ret _s_member_name (Bridge <TypeCode>* _b, ULong index,
		Interface* _env)
	{
		if (index >= Type <E>::count_) {
			set_Bounds (_env);
			return Type <String>::ret ();
		} else
			return const_string_ret_p (members_ [index]);
	}

	static Boolean equal (I_ptr <TypeCode> other) noexcept
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equal (TCKind::tk_enum, Base::RepositoryType::id,
				TypeCodeName <E>::name_, members_, Type <E>::count_, other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other) noexcept
	{
		return Base::_bridge () == &other ||
			TypeCodeBase::equivalent_ (TCKind::tk_enum, Base::RepositoryType::id,
				Type <E>::count_, TypeCodeBase::dereference_alias (other)) != TypeCodeBase::EqResult::NO;
	}

	static Type <CORBA::TypeCode>::VRet get_compact_typecode ()
	{
		return g_ORB->create_enum_tc (RepIdOf <E>::id, IDL::String (), EnumMemberSeq (Type <E>::count_));
	}

private:
	static const Char* const members_ [Type <E>::count_];
};

}
}

#endif
