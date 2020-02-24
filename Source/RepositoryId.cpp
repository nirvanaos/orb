#include <Nirvana/NirvanaBase.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/String.h>
#include <string.h>
#include <algorithm>

namespace CORBA {
namespace Nirvana {

using namespace std;

bool RepositoryId::compatible (const Char* current, String_in requested)
{
	return compatible (current, current ? strlen (current) : 0, requested);
}

bool RepositoryId::compatible (const Char* current, size_t current_len, String_in requested)
{
	const String& req_s = Type <String>::in (&requested);
	const Char* req_p = req_s.c_str ();
	
	if (current == req_p)
		return true;

	size_t req_l = req_s.length ();
	
	static const Char IDL [] = "IDL";
	static const size_t IDL_len = countof (IDL) - 1;
	
	if (is_type (current, IDL, IDL_len)) {
		if (is_type (req_p, IDL, IDL_len)) {
			const Char* minor = minor_version (current + IDL_len, current_len - IDL_len);
			const Char* r_minor = minor_version (req_p + IDL_len, req_l - IDL_len);
			if (minor - current == r_minor - req_p && equal (current, minor, req_p))
				return minor_number (minor) >= minor_number (r_minor);
			else
				return false;	// Type or major versions differ
		} else
			return false;
	}
	return current_len == req_l && equal (current, current + current_len, req_p);
}

bool RepositoryId::is_type (const Char* id, const Char* prefix, size_t cc)
{
	return equal (id, id + cc, prefix) && (id [cc] == ':');
}

const Char* RepositoryId::minor_version (const Char* id, size_t cc)
{
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