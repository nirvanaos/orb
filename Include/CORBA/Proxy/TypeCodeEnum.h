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

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <class E>
class TypeCodeEnum :
	public TypeCodeStatic <TypeCodeEnum <E>, TypeCodeWithId <tk_enum, RepIdOf <E> >, TypeCodeOps <E> >,
	public TypeCodeMemberCount <Type <E>::count_>
{
public:
	using TypeCodeMemberCount <Type <E>::count_>::_member_count;

	static const char* _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return name_;
	}

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, Interface* _env)
	{
		if (index >= Type <E>::count_) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else
			return members_ [index];
	}

private:
	static const Char* const members_ [Type <E>::count_];
	static const Char name_ [];
};

}
}

#endif
