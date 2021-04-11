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
#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>
#include <algorithm>

namespace CORBA {

using namespace Nirvana;
using namespace std;

#define DEFINE_SYSTEM_EXCEPTION(E)\
const E* E::_downcast (const Exception* ep) NIRVANA_NOEXCEPT { return (ep && (EC_##E == ep->__code ())) ? static_cast <const E*> (ep) : 0; }\
const char* E::_rep_id () const NIRVANA_NOEXCEPT { return RepIdOf <E>::repository_id_; }\
GNU_OPTNONE ::CORBA::TypeCode_ptr E::__type_code () const NIRVANA_NOEXCEPT { return _tc_##E; }

#define EXCEPTION_ENTRY(E) { { Nirvana::RepIdOf <E>::repository_id_, sizeof (E), ::CORBA::Nirvana::construct <E> }, countof (Nirvana::RepIdOf <E>::repository_id_) - 1 },

SYSTEM_EXCEPTIONS (DEFINE_SYSTEM_EXCEPTION)

const SystemException::ExceptionEntry SystemException::exception_entries_ [SystemException::KNOWN_SYSTEM_EXCEPTIONS] = {
	SYSTEM_EXCEPTIONS (EXCEPTION_ENTRY)
};

const Nirvana::ExceptionEntry* SystemException::_get_exception_entry (String_in rep_id, Code hint)
{
	struct Pred
	{
		bool operator () (const ExceptionEntry& l, String_in r) const
		{
			return RepositoryId::compare (l.ee.rep_id, l.rep_id_len, r) < 0;
		}

		bool operator () (String_in l, const ExceptionEntry& r) const
		{
			return RepositoryId::compare (r.ee.rep_id, r.rep_id_len, l) > 0;
		}
	};

	if (hint >= 0 && hint < KNOWN_SYSTEM_EXCEPTIONS) {
		const ExceptionEntry* pe = exception_entries_ + hint;
		if (Nirvana::RepositoryId::compatible (pe->ee.rep_id, pe->rep_id_len, rep_id))
			return &(pe->ee);
		hint = EC_SYSTEM_EXCEPTION;
	}

	const ExceptionEntry* pe = lower_bound (begin (exception_entries_), end (exception_entries_), rep_id, Pred ());
	if (pe != end (exception_entries_)) {
		if (Nirvana::RepositoryId::compatible (pe->ee.rep_id, pe->rep_id_len, rep_id))
			return &(pe->ee);
	}

	if (Exception::EC_SYSTEM_EXCEPTION == hint)
		return &exception_entries_ [EC_UNKNOWN].ee;
	else
		return nullptr;
}

const Nirvana::ExceptionEntry* SystemException::_get_exception_entry (TypeCode_ptr tc)
{
	if (tc && tc->kind () == tk_except)
		return _get_exception_entry (tc->id ());
	else
		return nullptr;
}

void SystemException::_raise_by_code (Code ec)
{
	if (ec >= 0 && ec < KNOWN_SYSTEM_EXCEPTIONS) {
		uint8_t buf [sizeof (SystemException)];
		(exception_entries_ [ec].ee.construct) (buf);
		((SystemException*)buf)->_raise ();
	} else
		throw UNKNOWN ();
}

void* SystemException::__data () NIRVANA_NOEXCEPT
{
	return &_data;
}

}
