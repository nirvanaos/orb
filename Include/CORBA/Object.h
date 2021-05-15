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

#include "AbstractBase.h"

/// CORBA::Object interface

//! \namespace	IOP
//!
//! \brief	Inter-ORB declarations.

namespace IOP {

struct IOR;

}

namespace PortableServer {

class ServantBase;

#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Internal::I_var <ServantBase> ServantBase_var;
#endif

}

namespace CORBA {

class ImplementationDef; // Not defined, unused
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <ImplementationDef> ImplementationDef_ptr;
typedef Internal::I_var <ImplementationDef> ImplementationDef_var;
#endif

typedef Object InterfaceDef; // TODO: Not defined yet
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <InterfaceDef> InterfaceDef_ptr;
typedef Internal::I_var <InterfaceDef> InterfaceDef_var;
#endif

extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_Object;

namespace Internal {

template <>
struct Type <PortableServer::ServantBase>;

template <class I>
struct TypeObject : TypeItf <I>
{
	static const CORBA::TCKind tc_kind = tk_objref;
};

template <>
const Char Bridge <Object>::repository_id_ [] = CORBA_REPOSITORY_ID ("Object");

template <>
struct Type <Object> : TypeItf <Object>
{
	static I_ptr <TypeCode> type_code ();
};

template <>
struct Bridge <Object>::EPV
{
	typedef Bridge <Object> MyBridge;

	Interface::EPV header;

	struct
	{
		NIRVANA_BASE_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
	} base;

	struct
	{
		Interface* (*get_interface) (Bridge <Object>*, Interface*);
		Type <Boolean>::ABI_ret (*is_a) (Bridge <Object>*,  Type <String>::ABI_in type_id, Interface*);
		Type <Boolean>::ABI_ret (*non_existent) (Bridge <Object>*, Interface*);
		Type <Boolean>::ABI_ret (*is_equivalent) (Bridge <Object>*, Interface*, Interface*);
		ULong (*hash) (Bridge <Object>*, ULong maximum, Interface*);
		// TODO: Other Object operations shall be here...
	} epv;

	struct
	{
		Interface* (*get_servant) (Bridge <Object>*, Interface*);
//		const IOP::IOR* (*object_reference) (ABI_in <Boolean> local);
	} internal;
};

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
};

template <class T>
Internal::I_ref <ImplementationDef> Client <T, Object>::_get_implementation ()
{
	return I_ref <ImplementationDef> ();
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

}

class Object : 
	public Internal::ClientInterfacePrimary <Object>,
	public Internal::ClientBase <Object, AbstractBase> // AbstractBase operations are not available directly on Object_ptr.
{};

inline Object::_ref_type AbstractBase::_to_object ()
{
	return _query_interface <Object> ();
}

namespace Internal {

// TODO: Move to Client.h
template <class Primary>
class ClientInterfaceBase <Primary, Object> :
	public Client <ClientBase <Primary, Object>, Object>
{
public:
	static I_ref <Primary> _narrow (Object::_ptr_type obj)
	{
		return AbstractBase::_ptr_type (obj)->_query_interface <Primary> ();
	}
};

}
}

#endif
