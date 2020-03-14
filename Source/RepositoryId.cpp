#include <Nirvana/NirvanaBase.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/String.h>
#include <string.h>
#include <algorithm>

namespace CORBA {
namespace Nirvana {

using namespace std;

RepositoryId::CheckResult RepositoryId::check (String_in current, String_in requested)
{
	const String& cur_s = static_cast <const String&> (current);
	return check (cur_s.c_str (), cur_s.length (), requested);
}

RepositoryId::CheckResult RepositoryId::check (const Char* current, size_t current_len, String_in requested)
{
	const String& req_s = static_cast <const String&> (requested);
	return check (current, current_len, req_s.c_str (), req_s.length ());
}

RepositoryId::CheckResult RepositoryId::check (const Char* current, size_t current_len, const Char* req_p, size_t req_l)
{
	if (current == req_p)
		return COMPATIBLE;
	
	static const Char IDL [] = "IDL";
	static const size_t IDL_len = countof (IDL) - 1;
	
	if (is_type (current, IDL, IDL_len)) {
		if (is_type (req_p, IDL, IDL_len)) {
			const Char* name = current + IDL_len + 1;
			const Char* r_name = req_p + IDL_len + 1;
			const Char* end = current + current_len;
			const Char* r_end = req_p + req_l;
			const Char* ver = version (name, end);
			const Char* r_ver = version (r_name, r_end);
			if (ver - name == r_ver - r_name && equal (name, ver, r_name)) {
				const Char* minor = minor_version (ver, end);
				const Char* r_minor = minor_version (r_ver, r_end);
				return minor_number (minor) >= minor_number (r_minor) ? COMPATIBLE : INCOMPATIBLE_VERSION;
			} 
		}
		return OTHER_INTERFACE;
	}
	return (current_len == req_l && equal (current, current + current_len, req_p)) ? COMPATIBLE : OTHER_INTERFACE;
}

bool RepositoryId::is_type (const Char* id, const Char* prefix, size_t cc)
{
	return equal (id, id + cc, prefix) && (id [cc] == ':');
}

const Char* RepositoryId::version (const Char* id, const Char* end)
{
	for (const Char* p = end - 1; p > id; --p) {
		if (':' == *p)
			return p;
	}
	return end;
}

const Char* RepositoryId::minor_version (const Char* ver, const Char* end)
{
	if (':' == *ver)
		return find (ver + 1, end, '.');
	return end;
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