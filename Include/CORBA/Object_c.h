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
#ifndef NIRVANA_ORB_OBJECT_C_H_
#define NIRVANA_ORB_OBJECT_C_H_
#pragma once

#include "Client.h"
#include "basic_types.h"
#include "TypeObject.h"

namespace CORBA {

class Object;

typedef Internal::Interface ImplementationDef; // Not defined, unused

class InterfaceDef;
class Context;
class NamedValue;
class NVList;
class Policy;
typedef ULong PolicyType;
typedef ULong Flags;
class Request;

namespace Internal {

template <> const Char RepIdOf <Object>::id [] = CORBA_REPOSITORY_ID ("Object");
NIRVANA_BRIDGE_BEGIN (CORBA::Object)
Interface* (*get_interface) (Bridge <Object>*, Interface*);
Type <Boolean>::ABI_ret (*is_a) (Bridge <Object>*, Type <String>::ABI_in, Interface*);
Type <Boolean>::ABI_ret (*non_existent) (Bridge <Object>*, Interface*);
Type <Boolean>::ABI_ret (*is_equivalent) (Bridge <Object>*, Interface*, Interface*);
ULong (*hash) (Bridge <Object>*, ULong maximum, Interface*);
void (*create_request) (Bridge <Object>*, Interface* ctx, Type <String>::ABI_in operation,
	Interface* arg_list, Interface* result, Interface** req, uint32_t req_flags, Interface*);
Interface* (*get_policy) (Bridge <Object>*, uint32_t, Interface*);
// TODO: Other Object operations shall be here...

Interface* (*query_interface) (Bridge <Object>*, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, Object> :
	public T
{
public:
	I_ref <ImplementationDef> _get_implementation ();
	I_ref <InterfaceDef> _get_interface ();
	Boolean _is_a (String_in type_id);
	Boolean _non_existent ();
	Boolean _is_equivalent (I_in <Object> other_object);
	ULong _hash (ULong maximum);
	void _create_request (I_ptr <Context> ctx, String_in operation, I_in <NVList> arg_list,
		I_in <NamedValue> result, I_out <Request> request, Flags req_flags);
	I_ref <Policy> _get_policy (PolicyType policy_type);

	// TODO: Other Object operations shall be here...

	/// This method does not increment reference counter
	I_ptr <Interface> _query_interface (String_in type_id);

	/// This method does not increment reference counter
	template <class I>
	I_ptr <I> _query_interface ()
	{
		return static_cast <I*> (&_query_interface (RepIdOf <I>::id));
	}
};

}
}

#endif
