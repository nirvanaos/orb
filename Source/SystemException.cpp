#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCode.h>
#include <algorithm>
#include <string.h>

#define EX_TABLE_ENTRY(e) { STATIC_BRIDGE (Nirvana::TypeCodeException <e>, TypeCode) },

namespace CORBA {

using namespace Nirvana;
using namespace std;

ExceptionEntry SystemException::type_codes_ [SystemException::KNOWN_SYSTEM_EXCEPTIONS] = {
	SYSTEM_EXCEPTIONS (EX_TABLE_ENTRY)
};

TypeCode_ptr SystemException::_get_type_code (const char* rep_id, int hint)
{
	struct TC_Pred
	{
		bool operator () (ExceptionEntry& l, const char* r) const
		{
			return strcmp (l->id (), r) < 0;
		}

		bool operator () (const char* l, ExceptionEntry& r) const
		{
			return strcmp (l, r->id ()) < 0;
		}
	};

	if (hint >= 0 && hint < KNOWN_SYSTEM_EXCEPTIONS) {
		TypeCode_ptr tc = type_codes_ [hint];
		if (Nirvana::RepositoryId::compatible (tc->id (), rep_id))
			return tc;
	} else {
		ExceptionEntry* pe = lower_bound (begin (type_codes_), end (type_codes_), rep_id, TC_Pred ());
		if (pe != end (type_codes_)) {
			TypeCode_ptr tc = *pe;
			if (Nirvana::RepositoryId::compatible (tc->id (), rep_id))
				return tc;
		}
	}

	return type_codes_ [EC_UNKNOWN];
}

}