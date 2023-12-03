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

namespace CORBA {
namespace Internal {

void raise_holder_exception (Bridge < ::Messaging::ExceptionHolder>& _b, Environment& _env,
	const IDL::Sequence <CORBA::Internal::ExceptionEntry>* exc_list)
{
	Type <bool>::ABI is_system_exception;
	const ExceptionEntry* user_exceptions;
	size_t user_exceptions_cnt;
	Type <IORequest>::C_ret exc ((_b._epv ().epv.get_exception) (&_b, &is_system_exception,
		&user_exceptions, &user_exceptions_cnt, &_env));
	_env.check ();

	IORequest::_ref_type rq (exc);

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
		if (exc_list) {
			user_exceptions = exc_list->data ();
			user_exceptions_cnt = exc_list->size ();
		}
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

