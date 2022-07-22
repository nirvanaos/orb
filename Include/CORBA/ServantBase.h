/// \file
/// ServantBase interface.
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
#ifndef NIRVANA_ORB_SERVANTBASE_H_
#define NIRVANA_ORB_SERVANTBASE_H_
#pragma once

#include "Object.h"

namespace PortableServer {

class POA;

extern const Nirvana::ImportInterfaceT <CORBA::TypeCode> _tc_Servant;

}

namespace CORBA {
namespace Internal {

template <>
struct Type <PortableServer::ServantBase> :
	TypeItfMarshalable <PortableServer::ServantBase>
{
	static I_ptr <TypeCode> type_code ()
	{
		return PortableServer::_tc_Servant;
	}

	static void marshal_in (I_ptr <PortableServer::ServantBase> src, IORequest_ptr rq);
	static void unmarshal (IORequest_ptr rq, I_ref <PortableServer::ServantBase>& dst);

};

// Type for native Servant
template <>
struct Type <PortableServer::Servant> : Type <PortableServer::ServantBase>
{};

template <> const Char RepIdOf <PortableServer::ServantBase>::id [] = PORTABLESERVER_REPOSITORY_ID ("ServantBase");
NIRVANA_BRIDGE_BEGIN (PortableServer::ServantBase)
Interface* (*default_POA) (Bridge <PortableServer::ServantBase>*, Interface*);
Interface* (*get_interface) (Bridge <PortableServer::ServantBase>*, Interface*);
Type <Boolean>::ABI_ret (*is_a) (Bridge <PortableServer::ServantBase>*, Type <String>::ABI_in type_id, Interface*);
Type <Boolean>::ABI_ret (*non_existent) (Bridge <PortableServer::ServantBase>*, Interface*);
void (*add_ref) (Bridge <PortableServer::ServantBase>*, Interface*);
void (*remove_ref) (Bridge <PortableServer::ServantBase>*, Interface*);
ULong (*refcount_value) (Bridge <PortableServer::ServantBase>*, Interface*);
void (*delete_object) (Bridge <PortableServer::ServantBase>*, Interface*);
Interface* (*query_interface) (Bridge <PortableServer::ServantBase>*, Type <String>::ABI_in, Interface*);
Interface* (*core_servant) (Bridge <PortableServer::ServantBase>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, PortableServer::ServantBase> :
	public T
{
public:
	I_ref <PortableServer::POA> _default_POA ();
	I_ref <InterfaceDef> _get_interface ();
	Boolean _is_a (String_in type_id);
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
		return _query_interface (RepIdOf <I>::id).template downcast <I> ();
	}

	PortableServer::Servant _core_servant ();
};

template <class T>
I_ref <PortableServer::POA> Client <T, PortableServer::ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <PortableServer::POA> _ret = (_b._epv ().epv.default_POA) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_ref <InterfaceDef> Client <T, PortableServer::ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, PortableServer::ServantBase>::_is_a (String_in type_id)
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_a) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, PortableServer::ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, PortableServer::ServantBase>::_add_ref ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.add_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, PortableServer::ServantBase>::_remove_ref ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.remove_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
ULong Client <T, PortableServer::ServantBase>::_refcount_value ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.refcount_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, PortableServer::ServantBase>::_delete_object ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.delete_object) (&_b, &_env);
	_env.check ();
}

template <class T>
I_ptr <Interface> Client <T, PortableServer::ServantBase>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.query_interface) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::PortableServer::Servant Client <T, ::PortableServer::ServantBase>::_core_servant ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_VT_ret <PortableServer::ServantBase> _ret = (_b._epv ().epv.core_servant) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace PortableServer {

class ServantBase :
	public CORBA::Internal::ClientInterfacePrimary <ServantBase>
{};

#ifdef LEGACY_CORBA_CPP

inline void release (Servant& ptr)
{
	::CORBA::release (ptr);
}

#endif

}

#endif
