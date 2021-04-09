/// \file AbstractBase.h
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

#include <Nirvana/NirvanaBase.h>
#include "Client.h"
#include "Type_interface.h"
#include "String.h"
#include "basic_types.h"
#include "TypeCode.h"

namespace CORBA {

class AbstractBase;
typedef Nirvana::I_ptr <AbstractBase> AbstractBase_ptr;
typedef Nirvana::I_var <AbstractBase> AbstractBase_var;
typedef Nirvana::I_out <AbstractBase> AbstractBase_out;
typedef Nirvana::I_inout <AbstractBase> AbstractBase_inout;

class Object;
typedef Nirvana::I_ptr <Object> Object_ptr;
typedef Nirvana::I_var <Object> Object_var;
typedef Nirvana::I_out <Object> Object_out;
typedef Nirvana::I_inout <Object> Object_inout;

class ValueBase;

namespace Nirvana {

// AbstractBase

BRIDGE_BEGIN (AbstractBase, CORBA_REPOSITORY_ID ("AbstractBase"))
	Interface* (*query_interface) (Bridge <AbstractBase>*, ABI_in <String>, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, AbstractBase> :
	public T
{
public:
	Interface* _query_interface (String_in type_id);

	template <class I>
	I_ptr <I> _query_interface ()
	{
		return static_cast <I*> (_query_interface (Bridge <I>::repository_id_));
	}
};

template <class T>
Interface* Client <T, AbstractBase>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <AbstractBase>& _b (T::_get_bridge (_env));
	Interface* _ret = (_b._epv ().epv.query_interface) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

}

class AbstractBase :
	public Nirvana::ClientInterfacePrimary <AbstractBase>
{
public:
	static AbstractBase_ptr _narrow (AbstractBase_ptr obj)
	{
		return _duplicate (obj);
	}

	inline Object_var _to_object ();
	inline ValueBase* _to_value ();
};

namespace Nirvana {

template <class Primary>
class ClientInterfaceBase <Primary, AbstractBase> :
	public Client <ClientBase <Primary, AbstractBase>, AbstractBase>
{
public:
	static I_ptr <Primary> _narrow (AbstractBase_ptr obj)
	{
		return Primary::_duplicate (obj->_query_interface <Primary> ());
	}
};

template <class I>
struct TypeAbstractInterface : TypeItf <I>
{
	static const TCKind tc_kind = tk_abstract_interface;
};

}
}

#endif
