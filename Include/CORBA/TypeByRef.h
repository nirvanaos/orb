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

#include "Type_forward.h"

namespace CORBA {
namespace Nirvana {

/// Data type, passed by reference.
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
/// \tparam TMember The struct member type.
template <typename T, typename TABI = T, typename TMember = T>
struct TypeByRef
{
	typedef T Var_type;
	typedef TABI ABI_type;

	// ABI data types
	typedef const ABI_type* ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;
	typedef const ABI_type* ABI_VT_ret;

	// Member types
	typedef TMember Member_type;
	typedef const TMember& MemberRef;

	// Client-side types

	class C_in
	{
	public:
		C_in (const Var_type& v) :
			ref_ (v)
		{}

		ABI_in operator & () const
		{
			return &reinterpret_cast <const ABI_type&> (ref_);
		}

	protected:
		const Var_type& ref_;
	};

	class C_inout
	{
	public:
		C_inout (Var_type& v) :
			ref_ (v)
		{}

		ABI_inout operator & () const
		{
			return &reinterpret_cast <ABI_type&> (ref_);
		}

	protected:
		Var_type& ref_;
	};

	typedef C_inout C_out;

	// Client T_var class for the legacy C++ IDL mapping support
	class C_var :
		public Var_type
	{
	public:
		C_var ()
		{}

		C_var (const Var_type& v) :
			Var_type (v)
		{}

		C_var (const C_var& src) :
			Var_type (src)
		{}

		C_var& operator = (const Var_type& v)
		{
			if (this != &v)
				Var_type::operator = (v);
			return *this;
		}

		C_var& operator = (const C_var& v)
		{
			if (this != &v)
				Var_type::operator = (v);
			return *this;
		}

		Var_type& operator -> ()
		{
			return *this;
		}

		const Var_type& operator -> () const
		{
			return *this;
		}

		C_in in () const
		{
			return *this;
		}

		C_out out ()
		{
			return *this;
		}

		C_inout inout ()
		{
			return *this;
		}

		Var_type _retn ()
		{
			return *this;
		}
	};

	struct C_ret : ABI_ret
	{
		operator const Var_type& () const
		{
			return reinterpret_cast <const Var_type&> (*this);
		}
	};

	class C_VT_ret
	{
	public:
		C_VT_ret (ABI_VT_ret p) :
			p_ (reinterpret_cast <const Var_type*> (p))
		{}

		operator MemberRef () const
		{
			_check_pointer (p_);
			return *p_;
		}

	protected:
		const Var_type* p_;
	};

	// Servant-side methods

	static const Var_type& in (ABI_in p)
	{
		_check_pointer (p);
		return reinterpret_cast <const Var_type&> (*p);
	}

	static Var_type& out (ABI_out p)
	{
		return inout (p);
	}

	static Var_type& inout (ABI_inout p)
	{
		_check_pointer (p);
		return reinterpret_cast <Var_type&> (*p);
	}

	static ABI_ret ret (Var_type& v)
	{
		return reinterpret_cast <ABI_ret&> (v);
	}

	static ABI_ret ret ()
	{
		return Var_type ();
	}

	static ABI_VT_ret VT_ret (const Var_type& v)
	{
		return &reinterpret_cast <const ABI_type&> (v);
	}

	static ABI_VT_ret VT_ret ()
	{
		return nullptr;
	}
};

}
}

#endif
