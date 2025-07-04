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
#ifndef NIRVANA_ORB_VALUEFACTORYBASE_H_
#define NIRVANA_ORB_VALUEFACTORYBASE_H_
#pragma once

#include "ValueBase.h"

namespace CORBA {

class ValueFactoryBase;

typedef Internal::I_ptr <ValueFactoryBase> ValueFactory;

#ifdef LEGACY_CORBA_CPP
typedef Internal::I_var <ValueFactoryBase> ValueFactoryBase_var;
#endif

namespace Core {

class RequestGIOP;
class RequestLocalBase;

}

namespace Internal {

template <>
struct Type <ValueFactoryBase> : TypeItf <ValueFactoryBase>
{};

// Type for native ValueFactory
template <>
struct Type <ValueFactory> : Type <ValueFactoryBase>
{};

template <> const Char RepIdOf <ValueFactoryBase>::id [] = CORBA_REPOSITORY_ID ("ValueFactoryBase");
NIRVANA_BRIDGE_BEGIN (ValueFactoryBase)
Interface* (*create_for_unmarshal) (Bridge <ValueFactoryBase>*, Interface*);
Interface* (*query_factory) (Bridge <ValueFactoryBase>*, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ValueFactoryBase> :
	public T
{
public:
	/// This method does not increment reference counter
	I_ptr <Interface> _query_factory (String_in type_id);

	/// This method does not increment reference counter
	template <class Itf>
	I_ptr <Itf> _query_factory ()
	{
		return static_cast <Itf*> (&_query_factory (RepIdOf <Itf>::id));
	}

private:
	Type <ValueBase>::VRet create_for_unmarshal ();
	friend class CORBA::Core::RequestGIOP;
	friend class CORBA::Core::RequestLocalBase;
};

template <class T>
Type <ValueBase>::VRet Client <T, ValueFactoryBase>::create_for_unmarshal ()
{
	Environment _env;
	Bridge <ValueFactoryBase>& _b (T::_get_bridge (_env));
	Type <ValueBase>::C_ret _ret = (_b._epv ().epv.create_for_unmarshal) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_ptr <Interface> Client <T, ValueFactoryBase>::_query_factory (String_in type_id)
{
	Environment _env;
	Bridge <ValueFactoryBase>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.query_factory) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

}

class ValueFactoryBase :
	public Internal::ClientInterfacePrimary <ValueFactoryBase>
{
public:
	static ValueFactoryBase::_ptr_type _downcast (ValueFactoryBase::_ptr_type f)
		noexcept
	{
		return f;
	}
};

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, ValueFactoryBase> :
	public Client <ClientBase <Primary, ValueFactoryBase>, ValueFactoryBase>
{
public:
	static I_ptr <Primary> _downcast (ValueFactoryBase::_ptr_type f)
	{
		if (f)
			return f->_query_factory <Primary> ();
		return nullptr;
	}
};

}
}

namespace IDL {

template <>
struct traits <CORBA::ValueFactoryBase> : CORBA::Internal::TraitsInterface <CORBA::ValueFactoryBase>
{};

}

#endif
