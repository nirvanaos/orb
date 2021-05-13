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

#include "AbstractBase.h"
#include "ReferenceCounter.h"

namespace PortableServer {

class POA;
typedef ::CORBA::Internal::I_ptr <POA> POA_ptr;
#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Internal::I_var <POA> POA_var;
#endif

class ServantBase;

}

namespace CORBA {
namespace Internal {

template <> class ::CORBA::Internal::I_ptr < ::PortableServer::ServantBase>;

}
}

namespace PortableServer {

typedef ::CORBA::Internal::I_ptr <ServantBase> Servant;

#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Internal::I_var <ServantBase> ServantBase_var;
#endif

}

namespace CORBA {

typedef Object InterfaceDef; // TODO: Not defined yet
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <InterfaceDef> InterfaceDef_ptr;
typedef Internal::I_var <InterfaceDef> InterfaceDef_var;
#endif

namespace Internal {

template <>
struct Type <PortableServer::ServantBase> : TypeItf <PortableServer::ServantBase>
{};

NIRVANA_BRIDGE_BEGIN (::PortableServer::ServantBase, PORTABLESERVER_REPOSITORY_ID ("ServantBase"))
NIRVANA_BASE_ENTRY (AbstractBase, CORBA_AbstractBase)
NIRVANA_BASE_ENTRY (ReferenceCounter, CORBA_Nirvana_ReferenceCounter)
NIRVANA_BRIDGE_EPV
Interface* (*default_POA) (Bridge < ::PortableServer::ServantBase>*, Interface*);
Interface* (*get_interface) (Bridge < ::PortableServer::ServantBase>*, Interface*);
Type <Boolean>::ABI_ret (*is_a) (Bridge < ::PortableServer::ServantBase>*, Type <String>::ABI_in type_id, Interface*);
Type <Boolean>::ABI_ret (*non_existent) (Bridge < ::PortableServer::ServantBase>*, Interface*);
Interface* (*core_servant) (Bridge < ::PortableServer::ServantBase>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::PortableServer::ServantBase> :
	public T
{
public:
	I_ref < ::PortableServer::POA> _default_POA ();
	I_ref <InterfaceDef> _get_interface ();
	Boolean _is_a (String_in type_id);
	Boolean _non_existent ();

	// Nirvana extension
	::PortableServer::Servant __core_servant ();
};

template <class T>
I_ref < ::PortableServer::POA> Client <T, ::PortableServer::ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <::PortableServer::POA> _ret = (_b._epv ().epv.default_POA) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_ref <InterfaceDef> Client <T, ::PortableServer::ServantBase>::_get_interface ()
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
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_a) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::PortableServer::ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::PortableServer::Servant Client <T, ::PortableServer::ServantBase>::__core_servant ()
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_VT_ret <::PortableServer::ServantBase> _ret = (_b._epv ().epv.core_servant) (&_b, &_env);
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

	I_ptr (const I_ref <::PortableServer::ServantBase>& var) NIRVANA_NOEXCEPT :
		I_ptr_base (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	I_ptr (I_ref <::PortableServer::ServantBase>&& var) NIRVANA_NOEXCEPT
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
	public ::CORBA::Internal::ClientInterface <ServantBase, ::CORBA::Internal::ReferenceCounter>,
	// Client methods from AbstractBase are not available directly on pointer to ServantBase.
	public ::CORBA::Internal::ClientBase <ServantBase, ::CORBA::AbstractBase>
{};

#ifdef LEGACY_CORBA_CPP

inline void release (Servant& ptr)
{
	::CORBA::release (ptr);
}

#endif

}

#endif
