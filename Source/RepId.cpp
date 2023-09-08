/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include "pch.h"
#include <algorithm>

namespace CORBA {
namespace Internal {

const Char RepId::IDL_ [] = "IDL";

RepId::Version::Version (const Char* sver) :
	major (0),
	minor (0)
{
	if (':' == *sver) {
		const Char* end;
		major = strtou16 (sver + 1, end);
		minor = minor_number (end);
	}
}

RepId::CheckResult RepId::check (String_in current, String_in requested)
{
	const String& cur_s = static_cast <const String&> (current);
	return check (cur_s.c_str (), cur_s.length (), requested);
}

RepId::CheckResult RepId::check (const Char* current, size_t current_len, String_in requested)
{
	const String& req_s = static_cast <const String&> (requested);
	return check (current, current_len, req_s.c_str (), req_s.length ());
}

RepId::CheckResult RepId::check (const Char* cur, size_t cur_l, const Char* req, size_t req_l)
{
	if (cur == req)
		return COMPATIBLE;

	const size_t IDL_len = countof (IDL_) - 1;

	if (is_type (cur, IDL_, IDL_len)) {
		if (is_type (req, IDL_, IDL_len)) {
			const Char* cur_name = cur + IDL_len + 1;
			const Char* cur_end = cur + cur_l;
			const Char* cur_minor = minor_version (version (cur_name, cur_end), cur_end);
			const Char* req_name = req + IDL_len + 1;
			const Char* req_end = req + req_l;
			const Char* req_minor = minor_version (version (req_name, req_end), req_end);
			if (cur_minor - cur_name == req_minor - req_name && std::equal (cur_name, cur_minor, req_name))
				return minor_number (cur_minor) >= minor_number (req_minor) ? COMPATIBLE : INCOMPATIBLE_VERSION;
		}
		return OTHER_INTERFACE;
	}
	return (cur_l == req_l && std::equal (cur, cur + cur_l, req)) ? COMPATIBLE : OTHER_INTERFACE;
}

bool RepId::is_type (const Char* id, const Char* prefix, size_t prefix_l)
{
	return std::equal (id, id + prefix_l, prefix) && (id [prefix_l] == ':');
}

const Char* RepId::version (const Char* begin, const Char* end)
{
	for (const Char* p = end - 1; p > begin; --p) {
		Char c = *p;
		assert (c); // Don't forget to exclude null terminating character from the length.
		if (':' == c)
			return p;
		else if (c != '.' && !('0' <= c && c <= '9'))
			break;
	}
	return end;
}

const Char* RepId::minor_version (const Char* ver, const Char* end)
{
	if (':' == *ver)
		return std::find (ver + 1, end, '.');
	return end;
}

uint_least16_t RepId::strtou16 (const Char* ver, const Char*& end)
{
	uint_least16_t n = 0;
	for (Char c; (c = *ver); ++ver) {
		if ('0' <= c && c <= '9') {
			if (n > USHRT_MAX / 10)
				::Nirvana::throw_INV_OBJREF ();
			n *= 10;
			uint_least16_t d = c - '0';
			if (d > USHRT_MAX - n)
				::Nirvana::throw_INV_OBJREF ();
			n += d;
		} else
			break;
	}
	end = ver;
	return n;
}

uint_least16_t RepId::minor_number (const Char* minor_version)
{
	if ('.' == *minor_version) {
		const Char* end;
		uint_least16_t n = strtou16 (minor_version + 1, end);
		assert (!*end || '.' == *end);
		return n;
	} else
		return 0;
}

int RepId::compare (const Char* cur, size_t cur_l, String_in requested)
{
	const String& req_s = static_cast <const String&> (requested);
	return compare (cur, cur_l, req_s.c_str (), req_s.length ());
}

int RepId::compare (const Char* cur, size_t cur_l, const Char* req, size_t req_l)
{
	const Char* req_end = req + req_l;
	const Char* cur_end = cur + cur_l;

	const size_t IDL_len = countof (IDL_) - 1;

	if (is_type (cur, IDL_, IDL_len) && is_type (req, IDL_, IDL_len)) {
		cur += IDL_len + 1;
		req += IDL_len + 1;
		const Char* cur_ver = version (cur, cur_end);
		const Char* req_ver = version (req, req_end);
		int ret = lex_compare (cur, cur_ver, req, req_ver);
		if (!ret)
			ret = Version (cur_ver).compare (Version (req_ver));
		return ret;
	} else
		return lex_compare (cur, cur_end, req, req_end);
}

int RepId::lex_compare (const Char* lhs, const Char* lhs_end, const Char* rhs, const Char* rhs_end)
{
	for (;;) {
		if (lhs == lhs_end) {
			if (rhs == rhs_end)
				return 0;
			else
				return -1;
		} else if (rhs == rhs_end)
			return 1;

		int ret = *lhs - *rhs;
		if (ret)
			return ret;
		++lhs;
		++rhs;
	}
}

}
}