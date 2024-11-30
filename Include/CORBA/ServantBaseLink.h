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
#ifndef NIRVANA_ORB_SERVANTBASELINK_H_
#define NIRVANA_ORB_SERVANTBASELINK_H_
#pragma once

#include "servant_core.h"
#include "LifeCycleRefCnt.h"

namespace PortableServer {
class POA;
}

namespace CORBA {

template <class> class servant_reference;

namespace Internal {

template <class, class> class Facet;

/// \brief Root base of the ServantBase implementations.
/// 
/// Provides interaction between the servant and core proxy object.
/// 
class ServantBaseLink :
	public BridgeVal <PortableServer::ServantBase>
{
public:
	// ServantBase operations

#ifdef LEGACY_CORBA_CPP
	I_ptr
#else
	I_ref
#endif
		<PortableServer::POA> _default_POA ()
	{
		return core_object ()->_default_POA ();
	}

	Type <InterfaceDef>::VRet _get_interface ()
	{
		return core_object ()->_get_interface ();
	}

	Boolean _is_a (String_in type_id)
	{
		return core_object ()->_is_a (type_id);
	}

	Boolean _non_existent () const noexcept
	{
		return false;
	}

	// Nirvana extension

	PortableServer::Servant _core_servant ()
	{
		return core_object ();
	}

	// Reference counter

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
	template <class, class> friend class Skeleton;
	template <class> friend class ServantPOA;
#endif
	template <class, class> friend class Facet;

	void _add_ref ()
	{
		core_object ()->_add_ref ();
	}

	void _remove_ref ()
	{
		core_object ()->_remove_ref ();
	}

public:
	ULong _refcount_value ()
	{
		return core_object ()->_refcount_value ();
	}

	Bridge <Object>* _get_object (Type <String>::ABI_in iid, Interface* env) noexcept
	{
		return get_object_from_core (core_object (), iid, env);
	}

protected:
	/// \brief Obtain `Object` interface pointer.
	/// 
	/// \returns `Object` interface from proxy.
	/// 
	/// Unlike the `_this()` method, `_object()` does not increment reference counter.
	/// It is intended for use inside servant implementation code only,
	/// for example, as parent CCM component reference for the facet creation.
	/// Outside the servant implementation code, `_this()` must be used instead.
	/// 
	Object::_ptr_type _object ();

protected:
	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& epv) :
		BridgeVal <PortableServer::ServantBase> (epv)
	{}

	ServantBaseLink (const ServantBaseLink&) = delete;

	ServantBaseLink& operator = (const ServantBaseLink&)
	{
		return *this; // Do nothing
	}

	void _create_proxy (Object::_ptr_type comp);

	Type <Interface>::VRet _get_proxy ()
	{
#ifdef LEGACY_CORBA_CPP
		return interface_duplicate (&get_proxy (core_object ()));
#else
		return get_proxy (core_object ());
#endif
	}

private:
	I_ptr <PortableServer::ServantBase> core_object ();

private:
	I_ref <PortableServer::ServantBase> core_object_;
};

}
}

#endif
