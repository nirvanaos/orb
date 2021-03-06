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
#ifndef NIRVANA_ORB_OBJECT_H_
#define NIRVANA_ORB_OBJECT_H_
#pragma once

#include "Client.h"
#include "Type_interface.h"
#include "String.h"
#include "basic_types.h"
#include "TCKind.h"

/// CORBA::Object interface

//! \namespace	IOP
//!
//! \brief	Inter-ORB declarations.

namespace IOP {

struct IOR;

}

namespace PortableServer {

class ServantBase;
typedef CORBA::Internal::I_ptr <ServantBase> Servant;

#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Internal::I_var <ServantBase> ServantBase_var;
#endif

}

namespace CORBA {
namespace Internal {

template <class I>
struct TypeObject : TypeItfMarshalable <I>
{
	static const TCKind tc_kind = TCKind::tk_objref;

	static void marshal_in (I_ptr <I> src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, I_ref <I>& dst);

};

}

class InterfaceDef;

}

namespace CORBA {

class Object;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <Object> Object_ptr;
typedef Internal::I_var <Object> Object_var;
#endif

typedef Internal::Interface ImplementationDef; // Not defined, unused

namespace Internal {

template <>
struct Type <PortableServer::Servant>;

template <>
struct Type <Object> : TypeObject <Object>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_Object;
	}
};

template <> const Char RepIdOf <Object>::id [] = CORBA_REPOSITORY_ID ("Object");
NIRVANA_BRIDGE_BEGIN (CORBA::Object)
Interface* (*get_interface) (Bridge <Object>*, Interface*);
Type <Boolean>::ABI_ret (*is_a) (Bridge <Object>*,  Type <String>::ABI_in type_id, Interface*);
Type <Boolean>::ABI_ret (*non_existent) (Bridge <Object>*, Interface*);
Type <Boolean>::ABI_ret (*is_equivalent) (Bridge <Object>*, Interface*, Interface*);
ULong (*hash) (Bridge <Object>*, ULong maximum, Interface*);
// TODO: Other Object operations shall be here...

Interface* (*query_interface) (Bridge <Object>*, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, Object> :
	public T
{
public:
	Internal::I_ref <ImplementationDef> _get_implementation ();
	Internal::I_ref <InterfaceDef> _get_interface ();
	Boolean _is_a (String_in type_id);
	Boolean _non_existent ();
	Boolean _is_equivalent (I_in <Object> other_object);
	ULong _hash (ULong maximum);
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

template <class T>
Internal::I_ref <ImplementationDef> Client <T, Object>::_get_implementation ()
{
	return I_ref <ImplementationDef> (); // Compatibility stub
}

template <class T>
Internal::I_ref <InterfaceDef> Client <T, Object>::_get_interface ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_a (String_in type_id)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_a) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_non_existent ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_equivalent (I_in <Object> other_object)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_equivalent) (&_b, &other_object, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, Object>::_hash (ULong maximum)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.hash) (&_b, maximum, &_env);
	_env.check ();
	return _ret;
}

// TODO: Other Object operations shall be here...

template <class T>
I_ptr <Interface> Client <T, Object>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.query_interface) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

}

class Object : 
	public Internal::ClientInterfacePrimary <Object>
{
public:
	static Internal::I_ref <Object> _narrow (Object::_ptr_type obj) NIRVANA_NOEXCEPT
	{
		return obj;
	}
};

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, Object> :
	public Client <ClientBase <Primary, Object>, Object>
{
public:
	static I_ref <Primary> _narrow (Object::_ptr_type obj)
	{
		if (obj)
			return obj->_query_interface <Primary> ();
		return nullptr;
	}

};

}
}

#endif
