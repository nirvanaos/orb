#ifndef NIRVANA_ORB_REPOSITORYID_H_
#define NIRVANA_ORB_REPOSITORYID_H_

#include "BasicTypes.h"

#define CORBA_REPOSITORY_ID(t) "IDL:omg.org/CORBA/" #t ":1.0"
#define NIRVANA_REPOSITORY_ID(t) "IDL:Nirvana/" #t ":1.0"

namespace CORBA {
namespace Nirvana {

class RepositoryId
{
public:
	static bool compatible (const Char* current, const Char* requested);

private:
	static bool is_type (const Char* id, const Char* prefix, size_t cc);
	static const Char* minor_version (const Char* id);
	static ULong minor_number (const Char* minor_version);
};

}
}

#endif
