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

using namespace CORBA;

namespace Messaging {

Internal::Bridge <ValueBase>* ExceptionHolder::_CORBA_ValueBase (Internal::Bridge <ExceptionHolder>* bridge,
	Internal::Type <IDL::String>::ABI_in id, Interface* env)
{
	if (!Internal::RepId::compatible (Internal::RepIdOf <ValueBase>::id, Internal::Type <IDL::String>::in (id)))
		Internal::set_INV_OBJREF (env);
	check_pointer (bridge, epv_.header);
	return &static_cast <ExceptionHolder&> (*bridge);
}

const Internal::Bridge <ExceptionHolder>::EPV ExceptionHolder::epv_ = {
	{ // header
		Internal::RepIdOf <ExceptionHolder>::id,
		ExceptionHolder::template __duplicate <ExceptionHolder>,
		ExceptionHolder::template __release <ExceptionHolder>
	},
	{ // base
		_CORBA_ValueBase
	}
};

void ExceptionHolder::__marshal (Internal::Bridge <ValueBase>*, Internal::Interface*,
	Internal::Interface* _env)
{
	Internal::set_NO_IMPLEMENT (_env);
}

void ExceptionHolder::__unmarshal (Internal::Bridge <ValueBase>*, Internal::Interface*,
	Internal::Interface* _env)
{
	Internal::set_NO_IMPLEMENT (_env);
}

void ExceptionHolder::raise_exception ()
{
	raise_exception (nullptr, 0);
}

void ExceptionHolder::raise_exception (const Internal::ExceptionEntry* user_exceptions,
	size_t user_exceptions_cnt) const
{
	std::aligned_storage <sizeof (SystemException), alignof (SystemException)>::type se;
	if (exception_ >>= reinterpret_cast <SystemException&> (se))
		reinterpret_cast <SystemException&> (se)._raise ();
	else {
		try {
			TypeCode::_ref_type tc = exception_.type ();
			const IDL::String rep_id = tc->id ();
			for (const Internal::ExceptionEntry* p = user_exceptions, *end = p + user_exceptions_cnt; p != end; ++p) {
				if (Internal::RepId::compatible (p->rep_id, rep_id)) {
					OctetSeq buf (p->size);
					p->construct (buf.data ());
					Exception& ex = *(Exception*)buf.data ();
					tc->n_copy (ex.__data (), exception_.data ());
					ex._raise ();
				}
			}
		} catch (...) {}
		Nirvana::throw_UNKNOWN ();
	}
}

}
