#include <CORBA/RepositoryId.h>
#include <Nirvana/Nirvana.h>
#include <algorithm>

namespace CORBA {
namespace Nirvana {

using namespace std;

bool RepositoryId::compatible (const Char* current, const Char* requested)
{
	if (current == requested)
		return true;

	static const Char IDL [] = "IDL";

	if (is_type (current, IDL, countof (IDL) - 1)) {
		if (is_type (requested, IDL, countof (IDL) - 1)) {
			const Char* minor = minor_version (current + countof (IDL) - 1);
			const Char* r_minor = minor_version (requested + countof (IDL) - 1);
			if (minor - current == r_minor - requested && equal (current, minor, requested))
				return minor_number (minor) >= minor_number (r_minor);
			else
				return false;	// Type or major versions differ
		} else
			return false;
	}
	return !strcmp (current, requested);
}

bool RepositoryId::is_type (const Char* id, const Char* prefix, size_t cc)
{
	return equal (id, id + cc, prefix) && (id [cc] == ':');
}

const Char* RepositoryId::minor_version (const Char* id)
{
	size_t cc = strlen (id);
	for (const Char* p = id + cc - 1; p > id; --p)
		if (':' == *p)
			return find (p + 1, id + cc, '.');
	return id + cc;
}

ULong RepositoryId::minor_number (const Char* minor_version)
{
	if ('.' == *minor_version)
		return strtoul (minor_version + 1, 0, 10);
	else
		return 0;
}

}
}