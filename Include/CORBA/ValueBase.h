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
#ifndef NIRVANA_ORB_VALUEBASE_H_
#define NIRVANA_ORB_VALUEBASE_H_
#pragma once

#include "AbstractBase.h"

namespace CORBA {

class ValueBase;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <ValueBase> ValueBase_ptr; // TODO: Old mapping requires the plain pointer here
typedef Internal::I_var <ValueBase> ValueBase_var;
typedef ValueBase_var& ValueBase_out;
#endif

namespace Internal {

// ValueBase

template <>
struct Type <ValueBase> : TypeItf <ValueBase>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_ValueBase;
	}
};

template <class I>
struct TypeValue : TypeItf <I>
{
	static const TCKind tc_kind = TCKind::tk_value;
};

NIRVANA_BRIDGE_BEGIN (ValueBase, CORBA_REPOSITORY_ID ("ValueBase"))
NIRVANA_BASE_ENTRY (AbstractBase, CORBA_AbstractBase)
NIRVANA_BRIDGE_EPV
Interface* (*copy_value) (Bridge <ValueBase>* _b, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ValueBase> :
	public T
{
public:
	Type <ValueBase>::Var _copy_value ();
};

template <class T>
Type <ValueBase>::Var Client <T, ValueBase>::_copy_value ()
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	Type <ValueBase>::C_ret _ret = (_b._epv ().epv.copy_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

}

class ValueBase :
	public Internal::ClientInterfacePrimary <ValueBase>,
	public Internal::ClientBase <ValueBase, AbstractBase> // AbstractBase operations are not available directly on Object_ptr.
{};

inline ValueBase::_ref_type AbstractBase::_to_value ()
{
	return _query_interface <ValueBase> ();
}

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, ValueBase> :
	public Client <ClientBase <Primary, ValueBase>, ValueBase>
{
public:
	static I_ref <Primary> _downcast (ValueBase::_ptr_type obj)
	{
		return AbstractBase::_ptr_type (obj)->_query_interface <Primary> ();
	}
};

}
}

#endif
