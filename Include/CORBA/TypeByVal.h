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
	typedef T Var;
	typedef TMember Member;
	typedef Var ConstRef; // By value

	// ABI data types
	typedef TABI ABI;
	typedef ABI ABI_in;
	typedef ABI* ABI_out;
	typedef ABI ABI_ret;
	typedef ABI ABI_VT_ret;

	static const bool has_check = false;
	static void check (const ABI&) {}

	// Client-side types

	// in parameters passed by value
	class C_in
	{
	public:
		C_in (Var val) :
			val_ (val)
		{}

		ABI_in operator & () const
		{
			return (ABI_in)val_;
		}

	private:
		Var val_;
	};

	typedef Var& C_out;
	typedef Var& C_inout;
	typedef Var C_ret;
	typedef Var C_VT_ret;

	// Servant-side methods

	static Var in (ABI_in v)
	{
		return v;
	}

	static Var& out (ABI_out p)
	{
		_check_pointer (p);
		return reinterpret_cast <Var&> (*p);
	}

	static Var& inout (ABI_out p)
	{
		_check_pointer (p);
		return reinterpret_cast <Var&> (*p);
	}

	static ABI_ret ret (Var v)
	{
		return (ABI_ret)v;
	}

	static ABI_ret ret ()
	{
		return ABI_ret ();
	}

	static ABI_VT_ret VT_ret (Var v)
	{
		return (ABI_ret)v;
	}

	static ABI_VT_ret VT_ret ()
	{
		return ABI_ret ();
	}

	static const bool has_marshal = false;

	static void marshal_in (Var src, Marshal_ptr marshaler, ABI& dst) NIRVANA_NOEXCEPT
	{
		dst = (ABI)src;
	}

	static void marshal_out (Var src, Marshal_ptr marshaler, ABI& dst) NIRVANA_NOEXCEPT
	{
		dst = (ABI)src;
	}

	static void unmarshal (ABI src, Unmarshal_ptr unmarshaler, Var& dst) NIRVANA_NOEXCEPT
	{
		dst = (Var)src;
	}
};

}
}

#endif
