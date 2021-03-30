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
#ifndef NIRVANA_ORB_TYPEBYVAL_H_
#define NIRVANA_ORB_TYPEBYVAL_H_

#include "Type_forward.h"

namespace CORBA {
namespace Nirvana {

/// Data type, passed by value.
template <class T>
struct TypeByVal
{
	typedef T ABI_type;

	static const bool has_check = false;
	static void check (const ABI_type&)
	{}

	// ABI data types
	typedef ABI_type ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;
	typedef ABI_type ABI_VT_ret;

	// Client-side types

	/// in parameters passed by value
	class C_in
	{
	public:
		C_in (T val) :
			val_ (val)
		{}

		ABI_in operator & () const
		{
			return val_;
		}

		// For member assignments
		operator T () const
		{
			return val_;
		}

	private:
		T val_;
	};

	typedef T& C_out;
	typedef T& C_inout;
	typedef T C_ret;
	typedef T C_VT_ret;

	// Servant-side methods

	static T in (ABI_in v)
	{
		return v;
	}

	static T& out (ABI_out p)
	{
		_check_pointer (p);
		return *p;
	}

	static T& inout (ABI_out p)
	{
		_check_pointer (p);
		return *p;
	}

	static ABI_ret ret (T v)
	{
		return v;
	}

	static ABI_VT_ret VT_ret (T v)
	{
		return v;
	}

	// Member types

	typedef T Member_type;
	typedef T Member_ret;
};

}
}

#endif
