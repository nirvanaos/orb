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
#ifndef NIRVANA_ORB_TYPEVARLEN_H_
#define NIRVANA_ORB_TYPEVARLEN_H_

#include "TypeByRef.h"
#include <utility>
#include <new>

namespace CORBA {
namespace Nirvana {

template <class T>
struct TypeVarLenBase : TypeByRef <T, ABI <T> >
{
	typedef TypeByRef <T, ABI <T> > Base;
	typedef typename Base::Var_type Var_type;
	typedef typename Base::C_in C_in;
	typedef typename Base::C_inout C_inout;
	typedef typename Base::ABI_ret ABI_ret;

	/// Variable-length types marshalling is always not trivial.
	static const bool has_marshal = true;

	/// C_out class clears output variable
	class C_out : public Base::C_out
	{
	public:
		C_out (Var_type& val) :
			Base::C_out (val)
		{
			val = Var_type ();	// Clear
		}
	};

	class C_ret
	{
	public:
		C_ret (ABI_ret&& val) :
			val_ (reinterpret_cast <T&&> (val))
		{}

		operator Var_type ()
		{
			return std::move (val_);
		}

	protected:
		Var_type val_;
	};

	// Client I_var class for the legacy C++ IDL mapping support
	class C_var :
		public Var_type
	{
	public:
		C_var ()
		{}

		C_var (const Var_type& v) :
			T (v)
		{}

		C_var (const C_var& src) :
			T (src)
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
			return std::move (static_cast <Var_type&> (*this));
		}
	};

	static ABI_ret ret (Var_type&& v)
	{
		ABI_ret abi;
		new (&abi) Var_type (std::move (v));
		return abi;
	}

	static ABI_ret ret ()
	{
		ABI_ret abi;
		new (&abi) Var_type ();
		return abi;
	}
};

template <class T, bool with_check> struct TypeVarLen;

template <class T>
struct TypeVarLen <T, false> : TypeVarLenBase <T>
{};

/// Base for variable-length data types
template <class T>
struct TypeVarLen <T, true> : TypeVarLenBase <T>
{
	typedef TypeVarLenBase <T> Base;
	typedef typename Base::Var_type Var_type;

	static const bool has_check = true;

	static void check_or_clear (Var_type& v);

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Var_type& val) :
			Base::C_inout (val)
		{}

		~C_inout () noexcept (false);
	};

	class C_out : public C_inout
	{
	public:
		C_out (Var_type& val) :
			C_inout (val)
		{
			val = Var_type ();	// Clear
		}
	};

	class C_ret : public Base::C_ret
	{
	public:
		C_ret (typename Base::ABI_ret&& val) :
			Base::C_ret (std::move (val))
		{
			check_or_clear (this->val_);
		}
	};

	class C_VT_ret : public Base::C_VT_ret
	{
	public:
		C_VT_ret (typename Base::ABI_VT_ret p) :
			Base::C_VT_ret (p)
		{
			Type <Var_type>::check (*p);
		}
	};

	// Servant-side methods

	static const Var_type& in (typename Base::ABI_in p)
	{
		_check_pointer (p);
		Type <Var_type>::check (*p);
		return reinterpret_cast <const Var_type&> (*p);
	}

	static Var_type& inout (typename Base::ABI_inout p)
	{
		_check_pointer (p);
		Type <Var_type>::check (*p);
		return reinterpret_cast <Var_type&> (*p);
	}

	static Var_type& out (typename Base::ABI_out p)
	{
		return inout (p);
	}
};

/// Outline for compact code
template <class T>
void TypeVarLen <T, true>::check_or_clear (Var_type& v)
{
	try {
		Type <Var_type>::check (reinterpret_cast <typename Type <Var_type>::ABI_type&> (v));
	} catch (...) {
		v.~Var_type (); // Destructor mustn't throw exceptions
		new (&v) Var_type ();
		throw;
	}
}

/// Outline for compact code
template <class T>
TypeVarLen <T, true>::C_inout::~C_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		check_or_clear (this->ref_);
	} catch (...) {
		if (!ex)
			throw;
	}
}

}
}

#endif
