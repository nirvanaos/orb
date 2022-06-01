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
#ifndef NIRVANA_ORB_LOCALOBJECT_H_
#define NIRVANA_ORB_LOCALOBJECT_H_
#pragma once

#include "Object.h"

namespace CORBA {

class LocalObject;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <LocalObject> LocalObject_ptr;
typedef Internal::I_var <LocalObject> LocalObject_var;
typedef LocalObject_var& LocalObject_out;
#endif

namespace Internal {

template <class I>
struct TypeLocalObject : TypeObject <I>
{
	static const TCKind tc_kind = TCKind::tk_local_interface;
};

template <>
struct Type <LocalObject> : TypeLocalObject <LocalObject>
{
	static const bool has_proxy = false;
};

NIRVANA_BRIDGE_BEGIN (LocalObject, CORBA_REPOSITORY_ID ("LocalObject"))
NIRVANA_BASE_ENTRY (CORBA::Object, CORBA_Object)
NIRVANA_BRIDGE_EPV
Type <Boolean>::ABI_ret (*non_existent) (Bridge <LocalObject>*, Interface*);
void (*add_ref) (Bridge <LocalObject>*, Interface*);
void (*remove_ref) (Bridge <LocalObject>*, Interface*);
ULong (*refcount_value) (Bridge <LocalObject>*, Interface*);
void (*delete_object) (Bridge <LocalObject>*, Interface*);
Interface* (*query_interface) (Bridge <LocalObject>*, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, LocalObject> :
	public T
{
public:
	Boolean _non_existent ();
	void _add_ref ();
	void _remove_ref ();
	ULong _refcount_value ();

	// Nirvana extensions
	
	void _delete_object ();

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
Boolean Client <T, LocalObject>::_non_existent ()
{
	Environment _env;
	Bridge <LocalObject>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, LocalObject>::_add_ref ()
{
	Environment _env;
	Bridge <LocalObject>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.add_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, LocalObject>::_remove_ref ()
{
	Environment _env;
	Bridge <LocalObject>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.remove_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
ULong Client <T, LocalObject>::_refcount_value ()
{
	Environment _env;
	Bridge <LocalObject>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.refcount_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, LocalObject>::_delete_object ()
{
	Environment _env;
	Bridge <LocalObject>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.delete_object) (&_b, &_env);
	_env.check ();
}

template <class T>
I_ptr <Interface> Client <T, LocalObject>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <LocalObject>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.query_interface) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

}

class LocalObject : public Internal::ClientInterface <LocalObject, Object>
{
public:
	using ClientInterfacePrimary <LocalObject>::_non_existent;
	using ClientInterfacePrimary <LocalObject>::_query_interface;

	static const bool _has_proxy = false;
};

}

#endif
