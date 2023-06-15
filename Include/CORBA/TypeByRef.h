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
#ifndef NIRVANA_ORB_TYPEBYREF_H_
#define NIRVANA_ORB_TYPEBYREF_H_
#pragma once

#include "TypeBase.h"

namespace CORBA {
namespace Internal {

/// Data type passed by reference.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI = T>
struct TypeByRefBase
{
	typedef T Var;
	typedef T VRet;
	typedef const Var& ConstRef;

	// ABI data types
	typedef TABI ABI;
	typedef const ABI* ABI_in;
	typedef ABI* ABI_out;
	typedef ABI ABI_ret;
	typedef const ABI* ABI_VT_ret;

	// Client-side types

	class C_in_ref
	{
	public:
		C_in_ref (const Var& v) :
			ref_ (v)
		{}

		ABI_in operator & () const
		{
			return &reinterpret_cast <const ABI&> (ref_);
		}

	protected:
		const Var& ref_;
	};

	typedef std::conditional_t <std::is_same <Var, ABI>::value, const Var&, C_in_ref> C_in;

	// Servant-side methods

	static ConstRef in (ABI_in p)
	{
		check_pointer (p);
		Type <T>::check (*p);
		return reinterpret_cast <const Var&> (*p);
	}

	static ABI_ret ret (Var&& v)
	{
		return (ABI_ret&&)v;
	}

	static ABI_ret ret ()
	{
		return ABI_ret ();
	}

	static ABI_VT_ret VT_ret (const Var& v) noexcept
	{
		return &reinterpret_cast <const ABI&> (v);
	}

	static ABI_VT_ret VT_ret () noexcept
	{
		return nullptr;
	}

	static const size_t CDR_size = sizeof (TABI);
	static const size_t CDR_align = alignof (TABI);
};

/// Data type passed by reference, without check.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI = T>
struct TypeByRef : TypeByRefBase <T, TABI>, TypeNoCheck <T, TABI>
{
	class C_inout
	{
	public:
		C_inout (T& v) :
			ref_ (v)
		{}

		TABI* operator & () const
		{
			return &reinterpret_cast <TABI&> (ref_);
		}

	protected:
		T& ref_;
	};

	typedef C_inout C_out;
	typedef T C_ret;

	typedef const T& C_VT_ret;

};

/// Data type passed by reference, with check.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI = T>
struct TypeByRefCheck : TypeByRefBase <T, TABI>, TypeWithCheck <T, TABI>
{
	class C_VT_ret
	{
	public:
		C_VT_ret (const TABI* p) :
			p_ (p)
		{}

		operator const T& () noexcept
		{
			check_pointer (p_);
			Type <T>::check (*p_);
			return reinterpret_cast <const T&> (*p_);
		}

	protected:
		const TABI* p_;
	};

};

}
}

#endif
