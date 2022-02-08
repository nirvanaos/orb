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
#ifndef NIRVANA_ORB_ABSTRACTBASE_H_
#define NIRVANA_ORB_ABSTRACTBASE_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include "Client.h"
#include "Type_interface.h"
#include "String.h"
#include "basic_types.h"
#include "TCKind.h"

namespace CORBA {

class AbstractBase;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <AbstractBase> AbstractBase_ptr;
typedef Internal::I_var <AbstractBase> AbstractBase_var;
typedef AbstractBase_var& AbstractBase_out;
#endif

class Object;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <Object> Object_ptr;
typedef Internal::I_var <Object> Object_var;
typedef Object_var& Object_out;
#endif

class ValueBase;

namespace Internal {

// AbstractBase

template <>
struct Type <AbstractBase> : TypeItf <AbstractBase>
{};

NIRVANA_BRIDGE_BEGIN (AbstractBase, CORBA_REPOSITORY_ID ("AbstractBase"))
	Interface* (*query_interface) (Bridge <AbstractBase>*, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, AbstractBase> :
	public T
{
public:
	/// This method does not increment reference counter
	I_ptr <Interface> _query_interface (String_in type_id);

	/// This method does not increment reference counter
	template <class I>
	I_ptr <I> _query_interface ()
	{
		return static_cast <I*> (&_query_interface (Bridge <I>::repository_id_));
	}
};

template <class T>
I_ptr <Interface> Client <T, AbstractBase>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <AbstractBase>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.query_interface) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

}

class AbstractBase :
	public Internal::ClientInterfacePrimary <AbstractBase>
{
public:
#ifdef LEGACY_CORBA_CPP
	static AbstractBase_ptr _narrow (AbstractBase_ptr obj)
	{
		return _duplicate (obj);
	}
#else
	static AbstractBase::_ref_type _narrow (AbstractBase::_ptr_type obj)
	{
		return obj;
	}
#endif

	inline Internal::I_ref <Object> _to_object ();
	inline Internal::I_ref <ValueBase> _to_value ();
};

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, AbstractBase> :
	public Client <ClientBase <Primary, AbstractBase>, AbstractBase>
{
public:
	static I_ref <Primary> _narrow (AbstractBase::_ptr_type obj)
	{
		return obj->_query_interface <Primary> ();
	}
};

template <class I>
struct TypeAbstractInterface : TypeItf <I>
{
	static const TCKind tc_kind = TCKind::tk_abstract_interface;
};

}
}

#endif
