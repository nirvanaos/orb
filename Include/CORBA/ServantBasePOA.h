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

namespace CORBA {
namespace Internal {

// POA implementation of PortableServer::ServantBase
template <>
class NIRVANA_NOVTABLE ServantPOA <PortableServer::ServantBase> :
	public ServantBaseLink,
	public virtual ServantPOA <AbstractBase>,
	public Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>
{
public:
	virtual Bridge <Object>* _get_object (String_in iid)
	{
		return ServantBaseLink::_get_object (iid);
	}

	// ServantBase operations

	virtual I_ref <PortableServer::POA> _default_POA ()
	{
		_check_construct ();
		return ServantBaseLink::_default_POA ();
	}

	virtual I_ref <InterfaceDef> _get_interface ()
	{
		_check_construct ();
		return ServantBaseLink::_get_interface ();
	}

	virtual Boolean _is_a (String_in type_id)
	{
		_check_construct ();
		return ServantBaseLink::_is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		return false;
	}

protected:
	ServantPOA ();

	virtual I_ref <Interface> _get_proxy ();

private:
	void _check_construct ()
	{
		if (!ServantBaseLink::core_object_)
			_construct ();
	}
};

}
}

#endif
