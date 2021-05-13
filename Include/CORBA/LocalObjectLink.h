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
#ifndef NIRVANA_ORB_LOCALOBJECTLINK_H_
#define NIRVANA_ORB_LOCALOBJECTLINK_H_

#include "local_core.h"

namespace CORBA {
namespace Internal {

//! \brief Implements delegate to the core LocalObject implementation.
class LocalObjectLink :
	public Bridge <LocalObject>
{
public:
	Bridge <Object>* _get_object (String_in iid) const
	{
		return get_object_from_core (core_object_, iid);
	}

	// Object operations

	Boolean _non_existent () const
	{
		return false;
	}

protected:
	LocalObjectLink (const Bridge <LocalObject>::EPV& epv) :
		Bridge <LocalObject> (epv)
	{}

	LocalObjectLink (const LocalObjectLink&) = delete;
	LocalObjectLink& operator = (const LocalObjectLink&)
	{
		return *this; // Do nothing
	}

	void _construct (Bridge <AbstractBase>& ab);

	Interface::_ref_type _get_proxy ()
	{
		return get_proxy (core_object_);
	}

protected:
	LocalObject::_ref_type core_object_;
};

}
}

#endif
