/// \file TypeCodeException.h
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
#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Nirvana {

template <class E, bool members = false>
class TypeCodeException :
	public TypeCodeStatic <TypeCodeException <E, members>, TypeCodeWithId <tk_except, RepIdOf <E> >, TypeCodeOps <typename E::_Data> >,
	public TypeCodeMembersOptional <E, members>
{
	typedef TypeCodeStatic <TypeCodeException <E, members>, TypeCodeWithId <tk_except, RepIdOf <E> >, TypeCodeOps <typename E::_Data> > Base;
	typedef TypeCodeMembersOptional <E, members> Members;
public:
	using Members::_member_count;
	using Members::_member_name;
	using Members::_member_type;

	static Type <String>::ABI_ret _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret_p (E::__name ());
	}

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return Base::equal (other)
			&& (other->name () == E::__name ())
			&& TypeCodeBase::equal (Members::members (), Members::member_count (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		I_ref <TypeCode> tco = TypeCodeBase::dereference_alias (other);
		return TypeCodeBase::equivalent (tk_except, RepIdOf <E>::repository_id_, tco)
			&& TypeCodeBase::equivalent (Members::members (), Members::member_count (), tco);
	}
};

}
}

#endif
