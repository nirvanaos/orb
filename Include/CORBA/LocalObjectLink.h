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
#pragma once

#include "local_core.h"

namespace CORBA {

template <class> class servant_reference;

namespace Internal {

//! \brief Implements delegate to the core LocalObject implementation.
class LocalObjectLink :
	public BridgeVal <LocalObject>
{
public:
	Bridge <Object>* _get_object (Type <String>::ABI_in iid, Interface* env) const NIRVANA_NOEXCEPT
	{
		return get_object_from_core (I_ptr <LocalObject> (core_object_), iid, env);
	}

	// Object operations

	Boolean _non_existent () const
	{
		return false;
	}

	// Reference counter

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
	template <class, class> friend class Skeleton;
#endif

	void _add_ref () const
	{
		core_object_->_add_ref ();
	}

	void _remove_ref () const
	{
		core_object_->_remove_ref ();
	}

	ULong _refcount_value () const
	{
		return core_object_->_refcount_value ();
	}

protected:
	LocalObjectLink (const Bridge <LocalObject>::EPV& epv) :
		BridgeVal <LocalObject> (epv)
	{}

	LocalObjectLink (const LocalObjectLink&) = delete;
	LocalObjectLink& operator = (const LocalObjectLink&)
	{
		return *this; // Do nothing
	}

	void _construct ();

	I_ref <Interface> _get_proxy () const
	{
		return get_proxy (I_ptr <LocalObject> (core_object_));
	}

protected:
	I_ref <LocalObject> core_object_;
};

}
}

#endif
