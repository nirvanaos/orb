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
#include <CORBA/ExceptionHolder.h>
#include <CORBA/CORBA.h>

namespace CORBA {
namespace Internal {

const Bridge <Messaging::ExceptionHolder>::EPV ExceptionHolderImpl::epv_ = {
	{ // header
		RepIdOf <Messaging::ExceptionHolder>::id,
		ExceptionHolderImpl::template __duplicate <Messaging::ExceptionHolder>,
		ExceptionHolderImpl::template __release <Messaging::ExceptionHolder>
	},
	{ // base
		_CORBA_ValueBase
	}
};

Bridge <ValueBase>* ExceptionHolderImpl::_CORBA_ValueBase (Bridge <Messaging::ExceptionHolder>* bridge,
	Type <IDL::String>::ABI_in id, Interface* env)
{
	if (!RepId::compatible (RepIdOf <ValueBase>::id, Type <IDL::String>::in (id)))
		set_INV_OBJREF (env);
	check_pointer (bridge, epv_.header);
	return &static_cast <ExceptionHolderImpl&> (*bridge);
}

void ExceptionHolderImpl::__marshal (Internal::Bridge <ValueBase>*, Internal::Interface*,
	Internal::Interface* _env)
{
	Internal::set_NO_IMPLEMENT (_env);
}

void ExceptionHolderImpl::__unmarshal (Internal::Bridge <ValueBase>*, Internal::Interface*,
	Internal::Interface* _env)
{
	Internal::set_NO_IMPLEMENT (_env);
}

void ExceptionHolderImpl::raise_exception (const ExceptionEntry* user_exceptions,
	size_t user_exceptions_cnt) const
{
	std::aligned_storage <sizeof (SystemException), alignof (SystemException)>::type se;
	if (exception_ >>= reinterpret_cast <SystemException&> (se))
		reinterpret_cast <SystemException&> (se)._raise ();
	else if (user_exceptions_cnt) {
		try {
			TypeCode::_ref_type tc = exception_.type ();
			const IDL::String rep_id = tc->id ();
			for (const ExceptionEntry* p = user_exceptions, *end = p + user_exceptions_cnt; p != end; ++p) {
				if (RepId::compatible (p->rep_id, rep_id)) {
					OctetSeq buf (p->size);
					p->construct (buf.data ());
					Exception& ex = *(Exception*)buf.data ();
					tc->n_copy (ex.__data (), exception_.data ());
					ex._raise ();
				}
			}
		} catch (...) {}
	}
	Nirvana::throw_UNKNOWN (MAKE_OMG_MINOR (1)); // Unlisted user exception received by client.
}

}
}

