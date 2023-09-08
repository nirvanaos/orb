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
#include <CORBA/Proxy/PollerBase.h>

namespace CORBA {
namespace Internal {

void PollerRoot::check_request (IORequest::_ptr_type rq, const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt)
{
	Any ex;
	if (!rq->get_exception (ex))
		return;
	std::aligned_storage <sizeof (SystemException), alignof (SystemException)>::type se;
	if (ex >>= reinterpret_cast <SystemException&> (se))
		reinterpret_cast <SystemException&> (se)._raise ();
	else {
		TypeCode::_ptr_type tc = ex.type ();
		IDL::String rep_id = tc->id ();
		for (const ExceptionEntry* p = user_exceptions, *end = p + user_exceptions_cnt; p != end; ++p) {
			if (RepId::compatible (p->rep_id, rep_id)) {
				if (p->size <= sizeof (se)) {
					(*p->construct) (&se);
					Exception* pex = (Exception*)&se;
					tc->n_move (pex->__data (), ex.data ());
					pex->_raise ();
				} else {
					std::vector <Octet> buf (p->size);
					(*p->construct) (buf.data ());
					Exception* pex = (Exception*)buf.data ();
					tc->n_move (pex->__data (), ex.data ());
					pex->_raise ();
				}
			}
		}
		throw UNKNOWN (MAKE_OMG_MINOR (1)); // Unlisted user exception received by client.
	}
}

Bridge <Messaging::Poller>* PollerRoot::messaging_poller (Type <String>::ABI_in id, Interface* env) const noexcept
{
	return aggregate_.get_bridge (id, env);
}

Bridge <Pollable>* PollerRoot::pollable (Type <String>::ABI_in id, Interface* env) const noexcept
{
	return pollable_.get_bridge (id, env);
}

Bridge <ValueBase>* PollerRoot::value_base (Type <String>::ABI_in id, Interface* env) const noexcept
{
	return value_base_.get_bridge (id, env);
}

}
}
