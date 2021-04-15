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
namespace Nirvana {

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.
/// So we use size_t (the machine word) as ABI for boolean in assumption that bool implementation can't be wide.
/// In and out parameters passed as reinterpret_cast <bool&> (size_t&).
/// Note that vector <bool> template specialization has element size is 1 byte.
typedef size_t ABI_boolean;

template <>
struct Type <Boolean> : TypeByVal <Boolean, ABI_boolean, Char>
{
	typedef TypeByVal <Boolean, ABI_boolean, Char> Base;
	typedef Boolean Var_type;
	typedef ABI_boolean ABI_type;
	typedef typename Base::ABI_inout ABI_inout;

	static const bool has_check = false;
	static void check (const ABI_type&) {}

	class C_inout
	{
	public:
		C_inout (Var_type& v) :
			ref_ (v),
			abi_ (v)
		{}

		~C_inout ()
		{
			ref_ = abi_;
		}

		ABI_inout operator & ()
		{
			return &abi_;
		}

	protected:
		Var_type& ref_;
		ABI_type abi_;
	};

	typedef C_inout C_out;

	typedef Boolean Member_ret;

	static TypeCode_ptr type_code ();

	static const bool has_marshal = false;

	static void marshal_in (Var_type src, Marshal_ptr marshaler, ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static void marshal_out (Var_type src, Marshal_ptr marshaler, ABI_type& dst) NIRVANA_NOEXCEPT
	{
		dst = src;
	}

	static void unmarshal (ABI_type src, Unmarshal_ptr unmarshaler, Var_type& dst) NIRVANA_NOEXCEPT
	{
		dst = src != 0;
	}
};

typedef Type <Boolean>::C_in Boolean_in;
typedef Type <Boolean>::C_out Boolean_out;
typedef Type <Boolean>::C_inout Boolean_inout;

}
}

#endif
