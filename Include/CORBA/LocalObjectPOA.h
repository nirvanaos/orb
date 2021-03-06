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
#ifndef NIRVANA_ORB_LOCALOBJECTPOA_H_
#define NIRVANA_ORB_LOCALOBJECTPOA_H_
#pragma once

#include "ServantBasePOA.h"
#include "LocalObjectLink.h"
#include "LocalObject_s.h"

#ifdef _MSC_BUILD
#pragma warning (disable:4250)
#endif

namespace CORBA {

template <class> class servant_reference;

namespace Internal {

template <>
class NIRVANA_NOVTABLE ServantPOA <LocalObject> :
	public LocalObjectLink,
	public Skeleton <ServantPOA <LocalObject>, LocalObject>,
	public virtual ServantPOA <PortableServer::ServantBase>
{
public:
	Bridge <Object>* _get_object (Type <String>::ABI_in iid, Interface* env);

	// Object operations

	virtual I_ref <PortableServer::POA> _default_POA ();
	virtual I_ref <InterfaceDef> _get_interface ();
	virtual Boolean _is_a (String_in type_id);
	virtual Boolean _non_existent ();

	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__non_existent;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__add_ref;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__remove_ref;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__refcount_value;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__delete_object;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__query_interface;

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
	friend class Skeleton <ServantPOA <LocalObject>, LocalObject>;
#endif

	virtual void _add_ref ()
	{
		_check_construct ();
		LocalObjectLink::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		_check_construct ();
		LocalObjectLink::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		_check_construct ();
		return LocalObjectLink::_refcount_value ();
	}

protected:
	ServantPOA () :
		LocalObjectLink (Skeleton <ServantPOA <LocalObject>, LocalObject>::epv_)
	{}

	virtual I_ref <Interface> _get_proxy ();

private:
	void _check_construct ()
	{
		if (!LocalObjectLink::core_object_)
			LocalObjectLink::_construct ();
	}
};

}
}

#endif
