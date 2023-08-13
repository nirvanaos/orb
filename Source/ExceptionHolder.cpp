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
#include <CORBA/Server.h>
#include <CORBA/ExceptionHolder.h>

namespace CORBA {
namespace Internal {

void raise_exception (IORequest::_ptr_type rq, bool is_system_exception,
	const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt)
{
	IDL::String id;
	Type <IDL::String>::unmarshal (rq, id);
	
	std::aligned_storage <sizeof (SystemException), alignof (SystemException)>::type small;
	OctetSeq large;

	Exception* ex = nullptr;
	if (is_system_exception) {
		const ExceptionEntry* ee = SystemException::_get_exception_entry (id, SystemException::EC_SYSTEM_EXCEPTION);
		if (!ee)
			throw UNKNOWN (MAKE_OMG_MINOR (2));  // Non-standard System Exception not supported.
		(ee->construct) (&small);
		ex = reinterpret_cast <Exception*> (&small);
	} else {
		for (const ExceptionEntry* ee = user_exceptions, *end = ee + user_exceptions_cnt; ee != end; ++ee) {
			if (RepId::compatible (ee->rep_id, id)) {
				if (ee->size <= sizeof (small)) {
					(ee->construct) (&small);
					ex = reinterpret_cast <Exception*> (&small);
				} else {
					large.resize (ee->size);
					ee->construct (large.data ());
					ex = reinterpret_cast <Exception*> (large.data ());
				}
				break;
			}
		}
		if (!ex)
			throw UNKNOWN (MAKE_OMG_MINOR (1)); // Unlisted user exception received by client.
	}
	assert (ex);
	ex->__type_code ()->n_unmarshal (rq, 1, ex->__data ());
	ex->_raise ();
}

}
}

