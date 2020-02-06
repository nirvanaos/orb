#include <CORBA/exceptions.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCodeException.h>

#define EX_TABLE_ENTRY(e) { e::repository_id_, e::_create },

namespace CORBA {

using namespace Nirvana;

const ExceptionEntry SystemException::creators_ [SystemException::KNOWN_SYSTEM_EXCEPTIONS] = {
	SYSTEM_EXCEPTIONS (EX_TABLE_ENTRY)
};

Exception* SystemException::_create (const char* rep_id, const void* data, int hint)
{
	const ExceptionEntry* entry = 0;
	if (hint >= 0 && hint < KNOWN_SYSTEM_EXCEPTIONS && Nirvana::RepositoryId::compatible (creators_ [hint].rep_id, rep_id))
		entry = creators_ + hint;
	else {
		for (const ExceptionEntry* pe = creators_; pe != creators_ + KNOWN_SYSTEM_EXCEPTIONS; ++pe)
			if (Nirvana::RepositoryId::compatible (pe->rep_id, rep_id)) {
				entry = pe;
				break;
			}
	}
	if (entry)
		return (entry->create) (data);
	else
		return new UNKNOWN ();
}

}