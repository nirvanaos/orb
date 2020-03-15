#include <Nirvana/NirvanaBase.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/String.h>
#include <string.h>
#include <algorithm>

namespace CORBA {
namespace Nirvana {

using namespace std;

const Char RepositoryId::IDL_ [] = "IDL";

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

RepositoryId::CheckResult RepositoryId::check (const Char* cur, size_t cur_l, const Char* req, size_t req_l)
{
	if (cur == req)
		return COMPATIBLE;

	const size_t IDL_len = countof (IDL_) - 1;

	if (is_type (cur, IDL_, IDL_len)) {
		if (is_type (req, IDL_, IDL_len)) {
			const Char* cur_name = cur + IDL_len + 1;
			const Char* cur_minor = minor_version (cur_name, cur + cur_l);
			const Char* req_name = req + IDL_len + 1;
			const Char* req_minor = minor_version (req_name, req + req_l);
			if (cur_minor - cur_name == req_minor - req_name && equal (cur_name, cur_minor, req_name))
				return minor_number (cur_minor) >= minor_number (req_minor) ? COMPATIBLE : INCOMPATIBLE_VERSION;
		}
		return OTHER_INTERFACE;
	}
	return (cur_l == req_l && equal (cur, cur + cur_l, req)) ? COMPATIBLE : OTHER_INTERFACE;
}

bool RepositoryId::is_type (const Char* id, const Char* prefix, size_t cc)
{
	return equal (id, id + cc, prefix) && (id [cc] == ':');
}

const Char* RepositoryId::version (const Char* begin, const Char* end)
{
	for (const Char* p = end - 1; p > begin; --p) {
		if (':' == *p)
			return p;
	}
	assert (false);
	return end;
}

const Char* RepositoryId::minor_version (const Char* begin, const Char* end)
{
	begin = version (begin, end);
	if (':' == *begin)
		return find (begin + 1, end, '.');
	return end;
}

ULong RepositoryId::minor_number (const Char* minor_version)
{
	assert ('.' == *minor_version);
	if ('.' == *minor_version) {
		Char* end;
		ULong ret = strtoul (minor_version + 1, &end, 10);
		assert (!*end);
		return ret;
	} else
		return 0;
}

int RepositoryId::compare (const Char* cur, size_t cur_l, String_in requested)
{
	const String& req_s = static_cast <const String&> (requested);
	const Char* req = req_s.c_str ();
	const Char* req_end = req + req_s.length ();
	const Char* cur_end = cur + cur_l;

	const size_t IDL_len = countof (IDL_) - 1;

	if (is_type (cur, IDL_, IDL_len) && is_type (req, IDL_, IDL_len)) {
		cur += IDL_len + 1;
		req += IDL_len + 1;
		return lexicographical_compare (cur, version (cur, cur_end), req, version (req, req_end));
	} else
		return lexicographical_compare (cur, cur_end, req, req_end);
}

}
}