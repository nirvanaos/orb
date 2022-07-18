/// \file
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
#pragma once

#include "TypeBase.h"

namespace CORBA {
namespace Internal {

/// Data type passed by value.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI = T>
struct TypeByValBase
{
	static const bool is_var_len = false;

	typedef T Var;
	typedef T VRet;
	typedef Var ConstRef; // By value

	// ABI data types
	typedef TABI ABI;
	typedef ABI ABI_in;
	typedef ABI* ABI_out;
	typedef ABI ABI_ret;
	typedef ABI ABI_VT_ret;

	// Client-side types

	// in parameters passed by value
	class C_in
	{
	public:
		C_in (const Var& v) NIRVANA_NOEXCEPT :
			val_ (v)
		{}

		ABI_in operator & () const NIRVANA_NOEXCEPT
		{
			return (ABI_in)val_;
		}

	private:
		Var val_;
	};

	// Servant-side methods

	static Var in (ABI abi)
	{
		Type <T>::check (abi);
		return (Var)abi;
	}

	static ABI_ret ret (Var v) NIRVANA_NOEXCEPT
	{
		return (ABI_ret)v;
	}

	static ABI_ret ret () NIRVANA_NOEXCEPT
	{
		return ABI_ret ();
	}

	static ABI_ret VT_ret (Var v) NIRVANA_NOEXCEPT
	{
		return (ABI_ret)v;
	}

	static ABI_ret VT_ret () NIRVANA_NOEXCEPT
	{
		return ABI_ret ();
	}

	// Marshaling

	static void marshal_out (Var& src, IORequest_ptr rq)
	{
		Type <T>::marshal_in (src, rq);
	}

	static void marshal_out_a (Var* src, size_t count, IORequest_ptr rq)
	{
		Type <T>::marshal_in_a (src, count, rq);
	}

};

/// Data type passed by value, without check().
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI = T>
struct TypeByVal : TypeByValBase <T, TABI>, TypeNoCheck <T, TABI>
{
	typedef T& C_out;
	typedef T& C_inout;
	typedef T C_ret;
	typedef T C_VT_ret;

};

/// Data type passed by value, with check.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI = T>
struct TypeByValCheck : TypeByValBase <T, TABI>, TypeWithCheck <T, TABI>
{
	class C_VT_ret
	{
	public:
		C_VT_ret (TABI abi) :
			abi_ (abi)
		{}

		operator T () NIRVANA_NOEXCEPT
		{
			Type <T>::check (abi_);
			return reinterpret_cast <T> (abi_);
		}

	protected:
		TABI abi_;
	};

};

}
}

#endif
