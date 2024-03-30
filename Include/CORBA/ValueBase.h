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

#include "Client.h"
#include "Type_interface.h"
#include "String.h"
#include "basic_types.h"
#include "TCKind.h"

namespace CORBA {

class ValueBase;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <ValueBase> ValueBase_ptr; // TODO: Old mapping requires the plain pointer here
typedef Internal::I_var <ValueBase> ValueBase_var;
#endif

class ValueFactoryBase;

namespace Internal {

// ValueBase

template <class I>
struct TypeValue : TypeItfMarshalable <I>
{
	inline static void marshal_in (I_ptr <I> src, IORequest_ptr rq);
	inline static void unmarshal (IORequest_ptr rq, I_ref <I>& dst);
};

template <>
struct Type <ValueBase> : TypeValue <ValueBase>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_ValueBase;
	}
};

template <> const Char RepIdOf <ValueBase>::id [] = CORBA_REPOSITORY_ID ("ValueBase");
NIRVANA_BRIDGE_BEGIN (ValueBase)
Interface* (*copy_value) (Bridge <ValueBase>* _b, Interface* _env);
ULong (*refcount_value) (Bridge <ValueBase>*, Interface*);
void (*marshal) (Bridge <ValueBase>* _b, Interface* rq, Interface* _env);
void (*unmarshal) (Bridge <ValueBase>* _b, Interface* rq, Interface* _env);
Interface* (*query_valuetype) (Bridge <ValueBase>*, Type <String>::ABI_in, Interface*);
Interface* (*truncatable_base) (Bridge <ValueBase>*, Interface*);
Interface* (*factory) (Bridge <ValueBase>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ValueBase> :
	public T
{
public:
#ifdef LEGACY_CORBA_CPP
	void _add_ref ();
	void _remove_ref ();
#endif

	/// Used to make a deep copy of the valuetype instance. The copy has no connections with
	/// the original instance and has a lifetime independent of that of the original.
	Type <ValueBase>::VRet _copy_value ();
	
	/// Returns the value of the reference count for the valuetype instance on which it is
	/// invoked.
	ULong _refcount_value ();

	/// Marshal value data to the interoperable request.
	/// This operation may not issue inter-domain calls.
	void _marshal (IORequest_ptr rq);

	/// Unmarshal value data from the interoperable request.
	/// This operation may issue inter-domain calls.
	void _unmarshal (IORequest_ptr rq);

	/// This method does not increment reference counter
	I_ptr <Interface> _query_valuetype (String_in type_id);

	/// This method does not increment reference counter
	template <class I>
	I_ptr <I> _query_valuetype ()
	{
		return _query_valuetype (RepIdOf <I>::id).template downcast <I> ();
	}

	/// If the value is truncatable, returns the type code of the truncatable base.
	/// Otherwise returns nil reference.
	I_ptr <TypeCode> _truncatable_base ();

	/// Returns factory interface for concrete value.
	/// For abstract value returns nil reference.
	I_ptr <ValueFactoryBase> _factory ();
};

template <class T>
Type <ValueBase>::VRet Client <T, ValueBase>::_copy_value ()
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	Type <ValueBase>::C_ret _ret = (_b._epv ().epv.copy_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ValueBase>::_marshal (IORequest_ptr rq)
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.marshal) (&_b, &rq, &_env);
	_env.check ();
}

template <class T>
void Client <T, ValueBase>::_unmarshal (IORequest_ptr rq)
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.unmarshal) (&_b, &rq, &_env);
	_env.check ();
}

template <class T>
I_ptr <Interface> Client <T, ValueBase>::_query_valuetype (String_in type_id)
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.query_valuetype) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_ptr <TypeCode> Client <T, ValueBase>::_truncatable_base ()
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	I_VT_ret <TypeCode> _ret = (_b._epv ().epv.truncatable_base) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_ptr <ValueFactoryBase> Client <T, ValueBase>::_factory ()
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	I_VT_ret <ValueFactoryBase> _ret = (_b._epv ().epv.factory) (&_b, &_env);
	_env.check ();
	return _ret;
}

#ifdef LEGACY_CORBA_CPP

template <class T>
void Client <T, ValueBase>::_add_ref ()
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	(_b._epv ().header.duplicate) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, ValueBase>::_remove_ref ()
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	(_b._epv ().header.release) (&_b);
}

#endif

template <class T>
ULong Client <T, ValueBase>::_refcount_value ()
{
	Environment _env;
	Bridge <ValueBase>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.refcount_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

}

class ValueBase :
	public Internal::ClientInterfacePrimary <ValueBase>
{
public:
	static ValueBase::_ptr_type _downcast (ValueBase::_ptr_type val) noexcept
	{
		return val;
	}
};

#ifdef LEGACY_CORBA_CPP

inline void add_ref (ValueBase::_ptr_type vb)
{
	if (vb)
		vb->_add_ref ();
}

inline void remove_ref (ValueBase::_ptr_type vb)
{
	if (vb)
		vb->_remove_ref ();
}

#endif

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, ValueBase> :
	public Client <ClientBase <Primary, ValueBase>, ValueBase>
{
public:
	static I_ptr <Primary> _downcast (ValueBase::_ptr_type val)
	{
		if (val)
			return val->_query_valuetype <Primary> ();
		return nullptr;
	}
};

}
}

namespace IDL {

template <>
struct traits <CORBA::ValueBase> : CORBA::Internal::TraitsInterface <CORBA::ValueBase>
{};

}

#endif
