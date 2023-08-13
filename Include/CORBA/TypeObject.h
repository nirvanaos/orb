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
#ifndef NIRVANA_ORB_TYPEOBJECT_H_
#define NIRVANA_ORB_TYPEOBJECT_H_
#pragma once

#include "Type_interface.h"
#include "IORequest.h"
#include "TCKind.h"
#include "tc_constants.h"

namespace CORBA {

class Object;
class LocalObject;

namespace Internal {

template <class I>
struct TypeObject : TypeItfMarshalable <I>
{
	static const TCKind tc_kind = TCKind::tk_objref;

	inline static void marshal_in (I_ptr <I> src, IORequest_ptr rq);
	inline static void unmarshal (IORequest_ptr rq, I_ref <I>& dst);
};

template <>
struct Type <Object> : TypeObject <Object>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_Object;
	}
};

template <class I>
struct TypeLocalObject : TypeObject <I>
{
	static const TCKind tc_kind = TCKind::tk_local_interface;
};

template <>
struct Type <LocalObject> : TypeLocalObject <LocalObject>
{};

}
}

#endif
