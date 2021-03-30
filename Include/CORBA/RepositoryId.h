/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_ORB_REPOSITORYID_H_
#define NIRVANA_ORB_REPOSITORYID_H_

#include "basic_types.h"
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
	static CheckResult check (const Char* current, size_t current_len, const Char* req_p, size_t req_l);

	static int compare (const Char* current, size_t current_len, String_in requested);
	static int compare (const Char* current, size_t current_len, const Char* req_p, size_t req_l);

	static const Char* version (const Char* id, const Char* end);

	struct Version
	{
		uint_least16_t major, minor;

		Version () {}
		Version (const Char* sver);

		int compare (const Version& rhs) const
		{
			int cmp = major - rhs.major;
			if (!cmp)
				cmp = minor - rhs.minor;
			return cmp;
		}

		bool operator < (const Version& rhs) const
		{
			return compare (rhs) < 0;
		}

		bool compatible (const Version& requested) const
		{
			return major == requested.major && minor >= requested.minor;
		}

		bool operator == (const Version& rhs) const
		{
			return major == rhs.major && minor == rhs.minor;
		}
	};

	static int lex_compare (const Char* lhs, const Char* lhs_end, const Char* rhs, const Char* rhs_end);

private:
	static bool is_type (const Char* id, const Char* prefix, size_t prefix_l);
	static const Char* minor_version (const Char* ver, const Char* end);
	static uint_least16_t minor_number (const Char* minor_version);
	static uint_least16_t strtou16 (const Char* ver, const Char*& end);
	
private:
	static const Char IDL_ [];
};

}
}

#endif
