/// \file TypeCodeMembers.h
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
#ifndef NIRVANA_ORB_TYPECODEMEMBERS_H_
#define NIRVANA_ORB_TYPECODEMEMBERS_H_

#include "TypeCodeImpl.h"
#include "InterfaceMetadata.h"

namespace CORBA {
namespace Nirvana {

template <class T>
class TypeCodeMembers
{
protected:
	static const Parameter members_ [];
};

template <ULong member_count, class Base, class Members>
class TypeCodeWithMembersImpl :
	public Base,
	public TypeCodeMemberCount <member_count>,
	public Members
{
public:
	using TypeCodeMemberCount <member_count>::_member_count;

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		if (index >= member_count) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else
			return Members::members_ [index].name;
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		if (index >= member_count) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else {
			TypeCode_ptr tc (Members::members_ [index].type);
			return (tc->_epv ().header.duplicate) (&tc, _env);
		}
	}
};

template <class T, ULong member_count, class Base>
class TypeCodeWithMembers :
	public TypeCodeWithMembersImpl <member_count, Base, TypeCodeMembers <T> >
{};

template <class T, class Base>
class TypeCodeWithMembers <T, 0, Base> :
	public Base,
	public TypeCodeMemberCount <0>
{
public:
	using TypeCodeMemberCount <0>::_member_count;

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		TypeCodeBase::set_Bounds (_env);
		return nullptr;
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		TypeCodeBase::set_Bounds (_env);
		return nullptr;
	}
};

}
}

#endif
