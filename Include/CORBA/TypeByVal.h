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
#ifndef NIRVANA_ORB_TYPEBYVAL_H_
#define NIRVANA_ORB_TYPEBYVAL_H_

#include "Type_forward.h"

namespace CORBA {
namespace Nirvana {

/// Data type, passed by value.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
/// \tparam TMember The struct member type.
template <typename T, typename TABI = T, typename TMember = T>
struct TypeByVal
{
	typedef T Var_type;
	typedef TABI ABI_type;

	// ABI data types
	typedef ABI_type ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;
	typedef ABI_type ABI_VT_ret;

	// Member types
	typedef TMember Member_type;
	typedef TMember Member_ret;

	// Client-side types

	// in parameters passed by value
	class C_in
	{
	public:
		C_in (Var_type val) :
			val_ (val)
		{}

		ABI_in operator & () const
		{
			return val_;
		}

		// For member assignments
		operator Member_type () const
		{
			return val_;
		}

	private:
		Var_type val_;
	};

	typedef Var_type& C_out;
	typedef Var_type& C_inout;
	typedef Var_type C_ret;
	typedef Var_type C_VT_ret;

	// Servant-side methods

	static Var_type in (ABI_in v)
	{
		return v;
	}

	static Var_type& out (ABI_out p)
	{
		_check_pointer (p);
		return reinterpret_cast <Var_type&> (*p);
	}

	static Var_type& inout (ABI_inout p)
	{
		_check_pointer (p);
		return reinterpret_cast <Var_type&> (*p);
	}

	static ABI_ret ret (Var_type v)
	{
		return v;
	}

	static ABI_ret ret ()
	{
		return Var_type ();
	}

	static ABI_VT_ret VT_ret (Var_type v)
	{
		return v;
	}

	static ABI_VT_ret VT_ret ()
	{
		return Var_type ();
	}
};

}
}

#endif
