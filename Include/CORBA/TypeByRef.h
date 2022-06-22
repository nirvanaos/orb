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

#include "Type_forward.h"

namespace CORBA {
namespace Internal {

/// Data type, passed by reference.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
/// \tparam TMember The struct member type.
template <typename T, typename TABI = T>
struct TypeByRef
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

	static const bool has_check = false;
	static void check (const ABI&) {}

	// Client-side types

	class C_in
	{
	public:
		C_in (const Var& v) :
			ref_ (v)
		{}

		ABI_in operator & () const
		{
			return &reinterpret_cast <const ABI&> (ref_);
		}

	protected:
		const Var& ref_;
	};

	class C_inout
	{
	public:
		C_inout (Var& v) :
			ref_ (v)
		{}

		ABI_out operator & () const
		{
			return &reinterpret_cast <ABI&> (ref_);
		}

	protected:
		Var& ref_;
	};

	typedef C_inout C_out;

	class C_VT_ret
	{
	public:
		C_VT_ret (ABI_VT_ret p) :
			p_ (reinterpret_cast <const Var*> (p))
		{}

		operator ConstRef () const
		{
			check_pointer (p_);
			return *p_;
		}

	protected:
		const Var* p_;
	};

	// Servant-side methods

	static ConstRef in (ABI_in p)
	{
		check_pointer (p);
		return reinterpret_cast <const Var&> (*p);
	}

	static Var& out (ABI_out p)
	{
		return inout (p);
	}

	static Var& inout (ABI_out p)
	{
		check_pointer (p);
		return reinterpret_cast <Var&> (*p);
	}

	static ABI_VT_ret VT_ret (const Var& v)
	{
		return &reinterpret_cast <const ABI&> (v);
	}

	static ABI_VT_ret VT_ret ()
	{
		return nullptr;
	}

};

}
}

#endif
