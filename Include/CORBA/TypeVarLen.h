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
#ifndef NIRVANA_ORB_TYPEVARLEN_H_
#define NIRVANA_ORB_TYPEVARLEN_H_
#pragma once

#include "TypeByRef.h"
#include "TypeVarLenHelper.h"
#include <utility>
#include <new>

namespace CORBA {
namespace Internal {

template <class T, class TABI>
struct TypeVarLenBase :
	TypeByRef <T, TABI>,
	TypeVarLenHelper <T, typename TypeByRef <T, TABI>::Var>
{
	static const bool fixed_len = false;

	typedef TypeByRef <T, TABI> Base;
	typedef typename Base::Var Var;
	typedef typename Base::C_in C_in;
	typedef typename Base::C_inout C_inout;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_ret ABI_ret;
	typedef typename Base::ABI ABI;

	// C_out class clears output variable
	class C_out : public Base::C_out
	{
	public:
		C_out (Var& val) :
			Base::C_out (val)
		{
			val = Var ();	// Clear
		}
	};

	class C_ret
	{
	public:
		C_ret (ABI_ret&& val) :
			val_ (reinterpret_cast <Var&&> (val))
		{}

		operator Var ()
		{
			return std::move (val_);
		}

	protected:
		Var val_;
	};

	// `const` is removed to let servant adopt the unmarshaled input data.
	static Var& in (ABI_in p)
	{
		check_pointer (p);
		return reinterpret_cast <const Var&> (const_cast <ABI&> (*p));
	}

	static ABI_ret ret (Var&& v)
	{
		ABI_ret abi;
		new (&abi) Var (std::move (v));
		return abi;
	}

	static ABI_ret ret ()
	{
		ABI_ret abi;
		new (&abi) Var ();
		return abi;
	}
};

template <class T, bool with_check, class TABI = ABI <T> > struct TypeVarLen;

template <class T, class TABI>
struct TypeVarLen <T, false, TABI> : TypeVarLenBase <T, TABI>
{
	static const bool has_check = false;
};

/// Base for variable-length data types
template <class T, class TABI>
struct TypeVarLen <T, true, TABI> : TypeVarLenBase <T, TABI>
{
	typedef TypeVarLenBase <T, TABI> Base;
	typedef typename Base::Var Var;

	static const bool has_check = true;

	static void check_or_clear (Var& v);

	class C_inout : public Base::C_inout
	{
	public:
		C_inout (Var& val) :
			Base::C_inout (val)
		{}

		~C_inout () noexcept (false);
	};

	class C_out : public C_inout
	{
	public:
		C_out (Var& val) :
			C_inout (val)
		{
			val = Var ();	// Clear
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
			Type <Var>::check (*p);
		}
	};

	// Servant-side methods

	static const Var& in (typename Base::ABI_in p)
	{
		check_pointer (p);
		Type <Var>::check (*p);
		return reinterpret_cast <const Var&> (*p);
	}

	static Var& inout (typename Base::ABI_out p)
	{
		check_pointer (p);
		Type <Var>::check (*p);
		return reinterpret_cast <Var&> (*p);
	}

	static Var& out (typename Base::ABI_out p)
	{
		return inout (p);
	}
};

/// Outline for compact code
template <class T, class TABI>
void TypeVarLen <T, true, TABI>::check_or_clear (Var& v)
{
	try {
		Type <Var>::check (reinterpret_cast <typename Type <Var>::ABI&> (v));
	} catch (...) {
		v.~Var (); // Destructor mustn't throw exceptions
		new (&v) Var ();
		throw;
	}
}

/// Outline for compact code
template <class T, class TABI>
TypeVarLen <T, true, TABI>::C_inout::~C_inout () noexcept (false)
{
	bool ex = uncaught_exception ();
	try {
		check_or_clear (this->ref_);
	} catch (...) {
		if (!ex)
			throw;
	}
}

/// Used in union destructors
template <class T> inline
void destruct (T& v) NIRVANA_NOEXCEPT
{
	v.~T ();
}

/// Used in union constructors
template <class T> inline
void construct (T& v) NIRVANA_NOEXCEPT
{
	::new (&v) T ();
}

}
}

#endif
