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

//! \brief Implements delegate to the core ServantBase implementation.
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
		_create_proxy ();
		return core_object_->_default_POA ();
	}

	Type <InterfaceDef>::VRet _get_interface ()
	{
		_create_proxy ();
		return core_object_->_get_interface ();
	}

	Boolean _is_a (String_in type_id)
	{
		_create_proxy ();
		return core_object_->_is_a (type_id);
	}

	Boolean _non_existent () const noexcept
	{
		return false;
	}

	// Our extensions

	PortableServer::Servant _core_servant ()
	{
		_create_proxy ();
		return core_object_;
	}

	// Reference counter

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
#endif
	template <class, class> friend class Skeleton;
	template <class> friend class ServantPOA;

	void _add_ref ()
	{
		_create_proxy ();
		core_object_->_add_ref ();
	}

	void _remove_ref ()
	{
		_create_proxy ();
		core_object_->_remove_ref ();
	}

public:
	ULong _refcount_value ()
	{
		_create_proxy ();
		return core_object_->_refcount_value ();
	}

	void _create_proxy ();

protected:
	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& epv) :
		BridgeVal <PortableServer::ServantBase> (epv)
	{}

	ServantBaseLink (const ServantBaseLink&) = delete;
	ServantBaseLink& operator = (const ServantBaseLink&)
	{
		return *this; // Do nothing
	}

	Type <Interface>::VRet _get_proxy ()
	{
		_create_proxy ();
#ifdef LEGACY_CORBA_CPP
		return interface_duplicate (&get_proxy (PortableServer::Servant (core_object_)));
#else
		return get_proxy (PortableServer::Servant (core_object_));
#endif
	}

protected:
	I_ref <PortableServer::ServantBase> core_object_;
};

}
}

#endif
