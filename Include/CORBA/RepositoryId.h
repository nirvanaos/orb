#ifndef NIRVANA_ORB_REPOSITORYID_H_
#define NIRVANA_ORB_REPOSITORYID_H_

#include "BasicTypes.h"
#include "StringBase.h"

namespace CORBA {
namespace Nirvana {

class RepositoryId
{
public:
	static bool compatible (const Char* current, String_in requested);
	static bool compatible (const Char* current, size_t current_len, String_in requested);

private:
	static bool is_type (const Char* id, const Char* prefix, size_t cc);
	static const Char* minor_version (const Char* id, size_t cc);
	static ULong minor_number (const Char* minor_version);
};

}
}

#endif
