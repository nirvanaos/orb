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
#ifndef NIRVANA_ORB_TYPECODESTRUCT_H_
#define NIRVANA_ORB_TYPECODESTRUCT_H_

#include "TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Internal {

template <class S>
class TypeCodeStruct :
	public TypeCodeStatic <TypeCodeStruct <S>, TypeCodeWithId <tk_struct, RepIdOf <S> >, TypeCodeOps <S> >,
	public TypeCodeMembers <S>,
	public TypeCodeName <S>
{
	typedef TypeCodeStatic <TypeCodeStruct <S>, TypeCodeWithId <tk_struct, RepIdOf <S> >, TypeCodeOps <S> > Base;
	typedef TypeCodeMembers <S> Members;
public:
	using TypeCodeName <S>::_name;
	using TypeCodeMembers <S>::_member_count;
	using TypeCodeMembers <S>::_member_name;
	using TypeCodeMembers <S>::_member_type;

	static Boolean equal (I_ptr <TypeCode> other)
	{
		if (!Base::equal (other))
			return false;
		if (!TypeCodeName <S>::equal (other))
			return false;
		return TypeCodeBase::equal (Members::members (), Members::member_count (), other);
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		I_ref <TypeCode> tco = TypeCodeBase::dereference_alias (other);
		if (!TypeCodeBase::equivalent (tk_struct, Base::RepositoryType::repository_id_, tco))
			return false;
		return TypeCodeBase::equivalent (Members::members (), Members::member_count (), tco);
	}
};

}
}

#endif
