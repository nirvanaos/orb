/// \file
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

#include <CORBA/Proxy/ProxyBase.h>

namespace CORBA {
namespace Internal {

bool call_request_proc (RqProcInternal proc, Interface* servant, Interface* call)
{
	IORequest::_ptr_type rq = IORequest::_nil ();
	try {
		rq = IORequest::_check (call);
		proc (servant, rq);
		rq->success ();
	} catch (Exception& e) {
		if (!rq)
			return false;

		Any any;
		any <<= std::move (e);
		rq->marshal_exception (any);
	}
	return true;
}

void ProxyRoot::check_request (IORequest::_ptr_type rq)
{
	Any ex;
	if (rq->unmarshal_exception (ex)) {
		std::aligned_storage <sizeof (SystemException), alignof (SystemException)>::type se;
		if (ex >>= reinterpret_cast <SystemException&> (se))
			reinterpret_cast <SystemException&> (se)._raise ();
		else
			throw UnknownUserException (std::move (ex));
	}
}

}
}
