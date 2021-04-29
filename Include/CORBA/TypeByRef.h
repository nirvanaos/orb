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
template <typename T, typename TABI = T>
struct TypeByRef
{
	typedef T Var;
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

	struct C_ret : ABI_ret
	{
		operator const Var& () const
		{
			return reinterpret_cast <const Var&> (*this);
		}
	};

	class C_VT_ret
	{
	public:
		C_VT_ret (ABI_VT_ret p) :
			p_ (reinterpret_cast <const Var*> (p))
		{}

		operator ConstRef () const
		{
			_check_pointer (p_);
			return *p_;
		}

	protected:
		const Var* p_;
	};

	// Servant-side methods

	static ConstRef in (ABI_in p)
	{
		_check_pointer (p);
		return reinterpret_cast <const Var&> (*p);
	}

	static Var& out (ABI_out p)
	{
		return inout (p);
	}

	static Var& inout (ABI_out p)
	{
		_check_pointer (p);
		return reinterpret_cast <Var&> (*p);
	}

	static ABI_ret ret (Var& v)
	{
		return reinterpret_cast <ABI_ret&> (v);
	}

	static ABI_ret ret ()
	{
		return Var ();
	}

	static ABI_VT_ret VT_ret (const Var& v)
	{
		return &reinterpret_cast <const ABI&> (v);
	}

	static ABI_VT_ret VT_ret ()
	{
		return nullptr;
	}

	static const bool has_marshal = false;

	static void marshal_in (const Var& src, Marshal_ptr marshaler, ABI& dst) NIRVANA_NOEXCEPT
	{
		dst = reinterpret_cast <const ABI&> (src);
	}

	static void marshal_out (Var& src, Marshal_ptr marshaler, ABI& dst) NIRVANA_NOEXCEPT
	{
		dst = reinterpret_cast <const ABI&> (src);
	}

	static void unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Var& dst) NIRVANA_NOEXCEPT
	{
		dst = reinterpret_cast <const Var&> (src);
	}

#ifdef LEGACY_CORBA_CPP

	// Client T_var class for the legacy C++ IDL mapping support
	class C_var :
		public Var
	{
	public:
		C_var ()
		{}

		C_var (const Var& v) :
			Var (v)
		{}

		C_var (Var&& v) :
			Var (std::move (v))
		{}

		NIRVANA_DEPRECATED
		C_var (Var* p) : // MyStruct_var v = new MyStruct();
			Var (*p)
		{
			delete p;
		}

		C_var (const C_var& src) :
			Var (src)
		{}

		C_var (C_var&& src) :
			Var (std::move (src))
		{}

		C_var& operator = (const Var& v)
		{
			Var::operator = (v);
			return *this;
		}

		C_var& operator = (Var&& v)
		{
			Var::operator = (std::move (v));
			return *this;
		}

		C_var& operator = (const C_var& src)
		{
			Var::operator = (src);
			return *this;
		}

		C_var& operator = (C_var&& src)
		{
			Var::operator = (std::move (src));
			return *this;
		}

		Var& operator -> ()
		{
			return *this;
		}

		const Var& operator -> () const
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

		Var _retn ()
		{
			return std::move (static_cast <Var&> (*this));
		}
	};

#endif
};

}
}

#endif
