/// \file TypeCodeException.h
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
#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Nirvana {

template <class E> class TypeCodeException;

template <class E>
class TypeCodeExceptionRoot :
	public TypeCodeStatic <TypeCodeException <E>, TypeCodeWithId <tk_except, E>, TypeCodeOps <typename E::_Data> >
{
public:
	static const char* _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return E::__name ();
	}
};

template <class E, ULong member_count = 0>
class TypeCodeExceptionImpl :
	public TypeCodeWithMembers <TypeCodeException <E>, member_count, TypeCodeExceptionRoot <E> >
{};

}
}

#endif
