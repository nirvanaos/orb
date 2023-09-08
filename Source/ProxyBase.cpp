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
#include "pch.h"
#include <CORBA/Proxy/ProxyBase.h>

namespace CORBA {
namespace Internal {

void ProxyRoot::check_request (IORequest::_ptr_type rq)
{
	Any ex;
	if (!rq->get_exception (ex))
		return;
	std::aligned_storage <sizeof (SystemException), alignof (SystemException)>::type se;
	if (ex >>= reinterpret_cast <SystemException&> (se))
		reinterpret_cast <SystemException&> (se)._raise ();
	else
		throw UnknownUserException (std::move (ex));
}

void ProxyRoot::set_marshal_local (Interface* env)
{
	// Attempt to marshal Local object.
	MARSHAL ex (MAKE_OMG_MINOR (4));
	set_exception (env, ex);
}

}
}
