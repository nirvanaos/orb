#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCode.h>
#include <algorithm>

#define EX_TABLE_ENTRY(e) { STATIC_BRIDGE (Nirvana::TypeCodeException <e>, TypeCode) },

namespace CORBA {

using namespace Nirvana;
using namespace std;

const ExceptionEntry SystemException::type_codes_ [SystemException::KNOWN_SYSTEM_EXCEPTIONS] = {
	SYSTEM_EXCEPTIONS (EX_TABLE_ENTRY)
};

TypeCode_ptr SystemException::_get_type_code (const char* rep_id, int hint)
{
	if (hint >= 0 && hint < KNOWN_SYSTEM_EXCEPTIONS) {
		TypeCode_ptr tc = type_codes_ [hint];
		if (Nirvana::RepositoryId::compatible (tc->id (), rep_id))
			return tc;
	} else {
		for (ExceptionEntry* pe = begin (type_codes_); pe != end (type_codes_); ++pe) {
			TypeCode_ptr tc = *pe;
			if (Nirvana::RepositoryId::compatible (tc->id (), rep_id))
				return tc;
		}
	}

	return type_codes_ [EC_UNKNOWN];
}

}