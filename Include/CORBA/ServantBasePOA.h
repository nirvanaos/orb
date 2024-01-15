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
#ifndef NIRVANA_ORB_SERVANTBASEPOA_H_
#define NIRVANA_ORB_SERVANTBASEPOA_H_
#pragma once

#include "AbstractBasePOA.h"
#include "ServantBaseLink.h"
#include "ServantBase_s.h"
#include "ServantMemory.h"

namespace CORBA {

template <class> class servant_reference;

namespace Internal {

// POA implementation of PortableServer::ServantBase
template <>
class NIRVANA_NOVTABLE ServantPOA <PortableServer::ServantBase> :
	public ServantBaseLink,
	public Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>,
	public virtual ServantPOA <AbstractBase>,
	public ServantMemory
{
public:
	virtual I_ptr <Interface> _query_interface (const String& id) = 0;

	// ServantBase operations

	virtual 
#ifdef LEGACY_CORBA_CPP
		I_ptr
#else
		I_ref
#endif
		<PortableServer::POA> _default_POA ()
	{
		return ServantBaseLink::_default_POA ();
	}

	virtual Type <InterfaceDef>::VRet _get_interface ()
	{
		return ServantBaseLink::_get_interface ();
	}

	virtual Boolean _is_a (String_in type_id);

	virtual Boolean _non_existent ()
	{
		return false;
	}

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
#endif
	friend class Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>;

	virtual void _add_ref () override
	{
		ServantBaseLink::_add_ref ();
	}

	virtual void _remove_ref () override
	{
		ServantBaseLink::_remove_ref ();
	}

public:
	virtual ULong _refcount_value () override;

protected:
	virtual void _delete_object () noexcept
	{
		delete this;
	}

protected:
	ServantPOA ();

	virtual ~ServantPOA ()
	{}

	virtual Type <Interface>::VRet _get_proxy ()
	{
		return ServantBaseLink::_get_proxy ();
	}

};

}
}

#endif
