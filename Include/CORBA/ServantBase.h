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

#include "POA_c.h"

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::PortableServer::ServantBase, PORTABLESERVER_REPOSITORY_ID ("ServantBase"))
BASE_STRUCT_ENTRY (AbstractBase, CORBA_AbstractBase)
BASE_STRUCT_ENTRY (ReferenceCounter, CORBA_Nirvana_ReferenceCounter)
BRIDGE_EPV
Interface* (*default_POA) (Bridge < ::PortableServer::ServantBase>*, Interface*);
Interface* (*get_interface) (Bridge < ::PortableServer::ServantBase>*, Interface*);
ABI_boolean (*is_a) (Bridge < ::PortableServer::ServantBase>*, ABI_in <String> type_id, Interface*);
ABI_boolean (*non_existent) (Bridge < ::PortableServer::ServantBase>*, Interface*);
Interface* (*core_servant) (Bridge < ::PortableServer::ServantBase>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ::PortableServer::ServantBase> :
	public T
{
public:
	::PortableServer::POA_var _default_POA ();
	InterfaceDef_var _get_interface ();
	Boolean _is_a (String_in type_id);
	Boolean _non_existent ();

	// Nirvana extension
	::PortableServer::Servant __core_servant ();
};

template <class T>
::PortableServer::POA_var Client <T, ::PortableServer::ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <::PortableServer::POA> _ret = (_b._epv ().epv.default_POA) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_var Client <T, ::PortableServer::ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::PortableServer::ServantBase>::_is_a (String_in type_id)
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.is_a) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::PortableServer::ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::PortableServer::Servant Client <T, ::PortableServer::ServantBase>::__core_servant ()
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <::PortableServer::ServantBase> _ret = (_b._epv ().epv.core_servant) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <> /// We can obtain I_ptr <ServantBase> directly from servant pointer
class I_ptr < ::PortableServer::ServantBase> : public I_ptr_base < ::PortableServer::ServantBase>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	/// We can obtain I_ptr directly from servant pointer
	I_ptr (Bridge < ::PortableServer::ServantBase>* p) NIRVANA_NOEXCEPT :
		I_ptr_base < ::PortableServer::ServantBase> (reinterpret_cast < ::PortableServer::ServantBase*> (p))
	{}

	I_ptr (const I_ptr& src) NIRVANA_NOEXCEPT :
		I_ptr_base (src)
	{}

	I_ptr (const I_var <::PortableServer::ServantBase>& var) NIRVANA_NOEXCEPT :
		I_ptr_base (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	I_ptr (I_var <::PortableServer::ServantBase>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}

	I_ptr& operator = (const I_ptr& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base::operator = (src);
		return *this;
	}

	/// When servant returns `I_ptr`, skeleton must be able to convert
	/// it to the ABI return type `Interface*`
	Bridge <::PortableServer::ServantBase>* operator & () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return reinterpret_cast <Bridge <::PortableServer::ServantBase>*> (this->p_);
	}
};

}
}

namespace PortableServer {

class ServantBase :
	public ::CORBA::Nirvana::ClientInterface <ServantBase, ::CORBA::Nirvana::ReferenceCounter>,
	// Client methods from AbstractBase are not available directly on pointer to ServantBase.
	public ::CORBA::Nirvana::ClientBase <ServantBase, ::CORBA::AbstractBase>
{};

inline void release (Servant& ptr)
{
	::CORBA::release (ptr);
}

}

#endif
