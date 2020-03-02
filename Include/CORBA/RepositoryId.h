#ifndef NIRVANA_ORB_REPOSITORYID_H_
#define NIRVANA_ORB_REPOSITORYID_H_

#include "BasicTypes.h"
#include "StringBase.h"

namespace CORBA {
namespace Nirvana {

class RepositoryId
{
public:
	enum CheckResult
	{
		COMPATIBLE,
		INCOMPATIBLE_VERSION,
		OTHER_INTERFACE
	};

	static bool compatible (String_in current, String_in requested)
	{
		return check (current, requested) == COMPATIBLE;
	}

	static bool compatible (const Char* current, size_t current_len, String_in requested)
	{
		return check (current, current_len, requested) == COMPATIBLE;
	}

	static CheckResult check (String_in current, String_in requested);
	static CheckResult check (const Char* current, size_t current_len, String_in requested);

private:
	static CheckResult check (const Char* current, size_t current_len, const Char* req_p, size_t req_l);
	static bool is_type (const Char* id, const Char* prefix, size_t cc);
	static const Char* minor_version (const Char* id, size_t cc);
	static ULong minor_number (const Char* minor_version);
};

}
}

#endif
