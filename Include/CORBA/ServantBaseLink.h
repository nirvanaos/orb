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

#include "servant_core.h"

namespace CORBA {
namespace Internal {

//! \brief Implements delegate to the core ServantBase implementation.
class ServantBaseLink :
	public Bridge <PortableServer::ServantBase>
{
public:
	Bridge <Object>* _get_object (String_in iid) const
	{
		return get_object_from_core (core_object_, iid);
	}

	// ServantBase operations

	I_ref <PortableServer::POA> _default_POA () const
	{
		return core_object_->_default_POA ();
	}

	I_ref <InterfaceDef> _get_interface () const
	{
		return core_object_->_get_interface ();
	}

	Boolean _is_a (String_in type_id) const
	{
		return core_object_->_is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return false;
	}

	// Our extensions
	Boolean _is_active () const
	{
		return !core_object_->_non_existent ();
	}

	PortableServer::Servant __core_servant () const
	{
		return core_object_;
	}

protected:
	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& epv) :
		Bridge <PortableServer::ServantBase> (epv)
	{}

	ServantBaseLink (const ServantBaseLink&) = delete;
	ServantBaseLink& operator = (const ServantBaseLink&)
	{
		return *this; // Do nothing
	}

	void _construct ();

	Interface::_ref_type _get_proxy ()
	{
		return get_proxy (core_object_);
	}

protected:
	PortableServer::ServantBase::_ref_type core_object_;
};

}
}

#endif
