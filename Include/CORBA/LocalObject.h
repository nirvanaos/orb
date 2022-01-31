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

#include "ReferenceCounter.h"

namespace CORBA {

class LocalObject;
#ifdef LEGACY_CORBA_CPP
template <> class Internal::I_ptr <LocalObject>;
typedef Internal::I_ptr <LocalObject> LocalObject_ptr;
typedef Internal::I_var <LocalObject> LocalObject_var;
typedef LocalObject_var& LocalObject_out;
#endif

namespace Internal {

template <>
struct Type <LocalObject> : TypeItf <LocalObject>
{
	static I_ptr <TypeCode> type_code ()
	{
		return _tc_Object;
	}
};

template <class I>
struct TypeLocalObject : TypeObject <I>
{
	static const TCKind tc_kind = TCKind::tk_local_interface;
};

NIRVANA_BRIDGE_BEGIN (LocalObject, CORBA_REPOSITORY_ID ("LocalObject"))
NIRVANA_BASE_ENTRY (CORBA::Object, CORBA_Object)
NIRVANA_BASE_ENTRY (ReferenceCounter, _ReferenceCounter)
NIRVANA_BRIDGE_EPV
Type <Boolean>::ABI_ret (*non_existent) (Bridge <LocalObject>*, Interface*);
NIRVANA_BRIDGE_END ()

template <> /// We can obtain I_ptr <LocalObject> directly from servant pointer
class I_ptr <LocalObject> : public I_ptr_base <LocalObject>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	/// We can obtain I_ptr directly from servant pointer
	I_ptr (Bridge <LocalObject>* p) NIRVANA_NOEXCEPT :
		I_ptr_base <LocalObject> (reinterpret_cast <LocalObject*> (p))
	{}

	I_ptr (const I_ptr& src) NIRVANA_NOEXCEPT :
		I_ptr_base (src)
	{}

	I_ptr (const I_ref <LocalObject>& var) NIRVANA_NOEXCEPT :
		I_ptr_base (var)
	{}

#ifdef LEGACY_CORBA_CPP

	I_ptr (const I_var <LocalObject>& var) NIRVANA_NOEXCEPT :
		I_ptr_base (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	I_ptr (I_var <LocalObject>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}

#endif

	I_ptr& operator = (const I_ptr& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base::operator = (src);
		return *this;
	}

	/// When servant returns `I_ptr`, skeleton must be able to convert
	/// it to the ABI return type `Interface*`
	Bridge <LocalObject>* operator & () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return reinterpret_cast <Bridge <LocalObject>*> (this->p_);
	}
};

template <class T>
class Client <T, LocalObject> :
	public T
{
public:
	Boolean _non_existent ();
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

}

class LocalObject : public Internal::ClientInterface < LocalObject, Object, Internal::ReferenceCounter >
{
public:
	using ClientInterfacePrimary <LocalObject>::_non_existent;
};

}

#endif
