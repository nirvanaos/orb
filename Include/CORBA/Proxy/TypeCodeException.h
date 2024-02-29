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
#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "../TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Internal {

template <class E>
class TypeCodeException :
	public TypeCodeStatic <TypeCodeException <E>,
		TypeCodeWithId <TCKind::tk_except, E>, TypeCodeOps <typename E::_Data> >,
	public TypeCodeORB,
	public TypeCodeMembers <E>
{
	typedef TypeCodeStatic <TypeCodeException <E>,
		TypeCodeWithId <TCKind::tk_except, E>, TypeCodeOps <typename E::_Data> > Base;
	typedef TypeCodeMembers <E> Members;

public:
	using Members::_s_member_count;
	using Members::_s_member_name;
	using Members::_s_member_type;
	using TypeCodeORB::_s_equal;
	using TypeCodeORB::_s_equivalent;
	using TypeCodeORB::_s_get_compact_typecode;

	static Type <String>::ABI_ret _s_name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret_p (E::__name ());
	}

};

}
}

#endif
