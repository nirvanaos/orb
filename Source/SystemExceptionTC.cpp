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
#include "../../pch/pch.h"
#include <CORBA/Proxy/TypeCodeException.h>
#include <CORBA/system_exceptions.h>

namespace CORBA {

namespace Internal {

template <>
const Parameter TypeCodeMembers <SystemException>::members_ [] = {
	{ "minor", Type <ULong>::type_code },
	{ "completed", Type <CompletionStatus>::type_code }
};

class TypeCodeSystemException :
	public TypeCodeStatic <TypeCodeSystemException, TypeCodeTK <TCKind::tk_except>, TypeCodeOps <SystemException::_Data> >,
	public TypeCodeMembers <SystemException>,
	public TypeCodeORB,
	public TypeCodeStaticIdName
{
public:
	using TypeCodeORB::_s_get_compact_typecode;
	using TypeCodeORB::_s_equal;
	using TypeCodeORB::_s_equivalent;
	using TypeCodeMembers <SystemException>::_s_member_count;
	using TypeCodeMembers <SystemException>::_s_member_name;
	using TypeCodeMembers <SystemException>::_s_member_type;
	using TypeCodeStaticIdName::_s_id;
	using TypeCodeStaticIdName::_s_name;
};

}

#define TC_EXCEPTION(E) extern NIRVANA_SELECTANY const Internal::StaticIdNameTC _tc_##E \
{&Internal::TypeCodeSystemException::epv_, Internal::RepIdOf <E>::id, #E};

SYSTEM_EXCEPTIONS (TC_EXCEPTION)

}
