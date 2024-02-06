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
#include "../../pch/pch.h"
#include <CORBA/LocalObjectPOA.h>

namespace CORBA {
namespace Internal {

ServantPOA <LocalObject>::ServantPOA () :
	LocalObjectLink (Skeleton <ServantPOA <LocalObject>, LocalObject>::epv_)
{}

void ServantPOA <LocalObject>::_create_proxy ()
{
	// Check that proxy is not yet created.
	// If proxy creation is in progress, the least significant bit is set.
	void*& obj = reinterpret_cast <void*&> (LocalObjectLink::core_object_);
	if (!obj)
		obj = (void*)(uintptr_t)1; // Set for interlocked construction
	else if (((uintptr_t)(void*)obj & 1) == 0)
		return; // Construction finished

	LocalObjectLink::_create_proxy (nullptr);
}

ULong ServantPOA <LocalObject>::_refcount_value ()
{
	_create_proxy ();
	return LocalObjectLink::_refcount_value ();
}

Boolean ServantPOA <LocalObject>::_is_a (String_in type_id)
{
	_create_proxy ();
	return LocalObjectLink::core_object_->_is_a (type_id);
}

Bridge <Object>* ServantPOA <LocalObject>::_get_object (Type <String>::ABI_in iid, Interface* env)
{
	_create_proxy ();
	return LocalObjectLink::_get_object (iid, env);
}

}
}
