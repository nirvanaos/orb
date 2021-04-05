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
#include "Type_forward.h"

namespace CORBA {

typedef bool Boolean;

namespace Nirvana {

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.
/// So we use size_t (the machine word) as ABI for boolean in assumption that bool implementation can't be wide.
/// Note that Sequence <bool> is implemented as vector <bool> template specialization
/// where element size is 1 byte.
typedef size_t ABI_boolean;

template <>
struct Type <Boolean>
{
	typedef ABI_boolean ABI_type;

	static const bool has_check = false;
	static void check (const ABI_type&) {}

	typedef ABI_type ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;
	typedef ABI_type ABI_VT_ret;

	// Member types
	typedef char Member_type;
	typedef Boolean Member_ret;

	class C_in
	{
	public:
		C_in (Boolean b) :
			b_ (b)
		{}

		ABI_type operator & () const
		{
			return b_;
		}

		// For member assignments
		operator Member_type () const
		{
			return (Member_type)b_;
		}

	private:
		Boolean b_;
	};

	class C_inout
	{
	public:
		C_inout (Boolean& b) :
			ref_ (b),
			abi_ (b)
		{}

		~C_inout ()
		{
			ref_ = abi_ != 0;
		}

		ABI_inout operator & ()
		{
			return &abi_;
		}

	protected:
		C_inout (Boolean& b, ABI_type init) :
			ref_ (b),
			abi_ (init)
		{}

	private:
		bool& ref_;
		ABI_type abi_;
	};

	class C_out : public C_inout
	{
	public:
		C_out (Boolean& b) :
			C_inout (b, 0)
		{}
	};

	class C_ret
	{
	public:
		C_ret (ABI_type abi) :
			b_ (abi != 0)
		{}

		operator Boolean () const
		{
			return b_;
		}

	private:
		Boolean b_;
	};

	typedef C_ret C_VT_ret;

	static bool in (ABI_in abi)
	{
		return abi != 0;
	}

	static bool& inout (ABI_inout p)
	{
		_check_pointer (p);
		return reinterpret_cast <bool&> (*p);
	}

	static bool& out (ABI_out p)
	{
		return inout (p);
	}

	static ABI_ret ret (Boolean b)
	{
		return b;
	}

	static ABI_VT_ret VT_ret (Boolean b)
	{
		return b;
	}
};

typedef Type <Boolean>::C_in Boolean_in;
typedef Type <Boolean>::C_out Boolean_out;
typedef Type <Boolean>::C_inout Boolean_inout;

}
}

#endif
