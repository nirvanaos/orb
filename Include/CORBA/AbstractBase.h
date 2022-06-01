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

#include "Object.h"
#include "ValueBase.h"

namespace CORBA {

class AbstractBase;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <AbstractBase> AbstractBase_ptr;
typedef Internal::I_var <AbstractBase> AbstractBase_var;
typedef AbstractBase_var& AbstractBase_out;
#endif

namespace Internal {

// AbstractBase

template <>
struct Type <AbstractBase> : TypeItf <AbstractBase>
{};

NIRVANA_BRIDGE_BEGIN (AbstractBase, CORBA_REPOSITORY_ID ("AbstractBase"))
Interface* (*to_object) (Bridge <AbstractBase>*, Interface*);
Interface* (*to_value) (Bridge <AbstractBase>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, AbstractBase> :
	public T
{
public:
	I_ref <Object> _to_object ();
	I_ref <ValueBase> _to_value ();
};

template <class T>
I_ref <Object> Client <T, AbstractBase>::_to_object ()
{
	Environment _env;
	Bridge <AbstractBase>& _b (T::_get_bridge (_env));
	I_ret <Object> _ret = (_b._epv ().epv.to_object) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_ref <ValueBase> Client <T, AbstractBase>::_to_value ()
{
	Environment _env;
	Bridge <AbstractBase>& _b (T::_get_bridge (_env));
	I_ret <ValueBase> _ret = (_b._epv ().epv.to_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

}

class AbstractBase :
	public Internal::ClientInterfacePrimary <AbstractBase>
{
public:
	static Internal::I_ref <AbstractBase> _narrow (AbstractBase::_ptr_type obj)
	{
		return obj;
	}

};

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, AbstractBase> :
	public Client <ClientBase <Primary, AbstractBase>, AbstractBase>
{
public:
	static I_ref <Primary> _narrow (AbstractBase::_ptr_type ab)
	{
		if (ab) {
			I_ref <Object> obj = ab->_to_object ();
			if (obj)
				return _narrow (obj);
			return _narrow (ab->_to_value ());
		}
		return nullptr;
	}

	static I_ref <Primary> _narrow (Object::_ptr_type obj)
	{
		if (obj)
			return obj->_query_interface <Primary> ();
		return nullptr;
	}

	static I_ref <Primary> _narrow (ValueBase::_ptr_type obj)
	{
		return obj->_query_valuetype <Primary> ();
	}

	static const bool _has_proxy = true;
};

template <class I>
struct TypeAbstractInterface : TypeItf <I>
{
	static const bool has_proxy = true;

	static const TCKind tc_kind = TCKind::tk_abstract_interface;

	static void marshal_in (I_ptr <I> src, IORequest_ptr rq);
	static void marshal_out (I_ref <I>& src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, I_ref <I>& dst);
};

}
}

#endif
