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
namespace Internal {

template <>
class NIRVANA_NOVTABLE ServantPOA <LocalObject> :
	public LocalObjectLink,
	public Skeleton <ServantPOA <LocalObject>, LocalObject>,
	public virtual ServantPOA <PortableServer::ServantBase>
{
public:
	virtual Bridge <Object>* _get_object (String_in iid);

	// Object operations

	virtual I_ref <PortableServer::POA> _default_POA ();
	virtual I_ref <InterfaceDef> _get_interface ();
	virtual Boolean _is_a (String_in type_id);
	virtual Boolean _non_existent ();
	virtual void _add_ref ();
	virtual void _remove_ref ();
	virtual ULong _refcount_value ();

	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__non_existent;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__add_ref;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__remove_ref;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__refcount_value;
	using Skeleton <ServantPOA <LocalObject>, LocalObject>::___delete_object;

protected:
	ServantPOA () :
		LocalObjectLink (Skeleton <ServantPOA <LocalObject>, LocalObject>::epv_)
	{}

	virtual I_ref <Interface> _get_proxy ();

private:
	void _check_construct ()
	{
		if (!LocalObjectLink::core_object_)
			LocalObjectLink::_construct (*this);
	}
};

}
}

#endif
