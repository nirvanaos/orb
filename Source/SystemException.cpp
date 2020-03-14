#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCode.h>
#include <Nirvana/OLF.h>
#include <algorithm>
#include <string.h>

#define DEFINE_SYS_EXCEPTION(E)\
Long E::__code () const NIRVANA_NOEXCEPT { return EC_##E; }\
const E* E::_downcast (const Exception* ep) NIRVANA_NOEXCEPT { return (ep && (EC_##E == ep->__code ())) ? static_cast <const E*> (ep) : 0; }\

#define DEFINE_SYSTEM_EXCEPTION(E) DEFINE_EXCEPTION1("omg.org/", CORBA, E, 1, 0) DEFINE_SYS_EXCEPTION(E)
#define DEFINE_NIRVANA_EXCEPTION(E) DEFINE_EXCEPTION1("", CORBA, E, 1, 0) DEFINE_SYS_EXCEPTION(E)

#define EXCEPTION_ENTRY(E) { { E::repository_id_, sizeof (E), ::CORBA::Nirvana::construct <E> }, countof (E::repository_id_) - 1 },

namespace CORBA {

using namespace Nirvana;
using namespace std;

CORBA_EXCEPTIONS (DEFINE_SYSTEM_EXCEPTION)
NIRVANA_EXCEPTIONS (DEFINE_NIRVANA_EXCEPTION)

const SystemException::ExceptionEntry SystemException::exception_entries_ [SystemException::KNOWN_SYSTEM_EXCEPTIONS] = {
	SYSTEM_EXCEPTIONS (EXCEPTION_ENTRY)
};

const Nirvana::ExceptionEntry* SystemException::_get_exception_entry (String_in rep_id, int hint)
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

	if (Exception::EC_SYSTEM_EXCEPTION == hint) {
		const ExceptionEntry* pe = lower_bound (begin (exception_entries_), end (exception_entries_), rep_id, Pred ());
		if (pe != end (exception_entries_)) {
			if (Nirvana::RepositoryId::compatible (pe->ee.rep_id, pe->rep_id_len, rep_id))
				return &(pe->ee);
		}
	}

	return &exception_entries_ [EC_UNKNOWN].ee;
}

}