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
#ifndef NIRVANA_ORB_SERVANT_CORE_H_
#define NIRVANA_ORB_SERVANT_CORE_H_

#include "Type_interface.h"
#include "ServantBase.h"

namespace CORBA {
namespace Internal {

// TODO: In the final implementation this method won't be called for servants.
// So we could replace the implementation to throw_BAD_OPERATION() call
// or to assert (false) and return nullptr.
inline Bridge <Object>* get_object_from_core (PortableServer::Servant core_object, String_in iid)
{
	return static_cast <Bridge <Object>*> (&AbstractBase::_ptr_type (core_object)->_query_interface (iid));
}

Interface::_ref_type get_proxy (PortableServer::Servant core_object);

}
}

#endif
