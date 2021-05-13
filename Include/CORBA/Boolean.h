/// \file Boolean.h
/// Provides ABI for the bool type.
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
#ifndef NIRVANA_ORB_BOOLEAN_H_
#define NIRVANA_ORB_BOOLEAN_H_

#include <Nirvana/NirvanaBase.h>
#include "TypeByVal.h"

namespace CORBA {
namespace Internal {

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.

template <>
struct Type <Boolean> : TypeByVal <Boolean, Char>
{
	class C_inout
	{
	public:
		C_inout (Var& v) :
			ref_ (v),
			abi_ (v)
		{}

		~C_inout ()
		{
			ref_ = abi_;
		}

		ABI_out operator & ()
		{
			return (Char*)&abi_;
		}

	protected:
		Var& ref_;
		// The ABI for boolean is Char.
		// But sizeof(bool) is implementation-dependent and might be > sizeof(char).
		// So we reserve size_t (the machine word) as ABI for boolean in assumption that bool implementation can't be wide.
		size_t abi_;
	};

	typedef C_inout C_out;

	static I_ptr <TypeCode> type_code ();

	// For the ABI compatibility, compiler uses char as structured type member of boolean type.
	// So we have 2 kinds unmarshal methods.

	static void unmarshal (ABI src, Unmarshal_ptr unmarshaler, Var& dst) NIRVANA_NOEXCEPT
	{
		dst = (Var)src;
	}

	static void unmarshal (ABI src, Unmarshal_ptr unmarshaler, Char& dst) NIRVANA_NOEXCEPT
	{
		dst = (Var)src;
	}
};

}
}

#endif
