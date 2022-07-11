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
#ifndef NIRVANA_ORB_TYPEBASE_H_
#define NIRVANA_ORB_TYPEBASE_H_
#pragma once

#include "Type_forward.h"

namespace CORBA {
namespace Internal {

/// Common base for all types (except for interface types).
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename ABI>
struct TypeBase
{
	// Servant-side methods

	static T& out (ABI* p)
	{
		return inout (p);
	}

	static T& inout (ABI* p)
	{
		check_pointer (p);
		return reinterpret_cast <T&> (*p);
	}

};

/// Type that does not have check()
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI>
struct TypeNoCheck : public TypeBase <T, TABI>
{
	static void check (const TABI&) {}
	static const bool has_check = false;

};

/// Type that has non-trivial check()
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI>
struct TypeWithCheck : public TypeBase <T, TABI>
{
	static const bool has_check = true;

	static void check_or_clear (TABI& v);

	class C_inout
	{
	public:
		C_inout (T& val) NIRVANA_NOEXCEPT :
			ref_ (reinterpret_cast <TABI&> (val))
		{}

		~C_inout () noexcept (false);

		TABI* operator & () const NIRVANA_NOEXCEPT
		{
			return &ref_;
		}

	protected:
		TABI& ref_;
	};

	typedef C_inout C_out;

	class C_ret
	{
	public:
		C_ret (TABI&& val) :
			val_ (val)
		{}

		operator T () NIRVANA_NOEXCEPT
		{
			check_or_clear (val_);
			return std::move (reinterpret_cast <T&> (val_));
		}

	protected:
		TABI val_;
	};

};

// Outline for compact code
template <typename T, typename TABI>
void TypeWithCheck <T, TABI>::check_or_clear (TABI& v)
{
	if (Type <T>::is_var_len) {
		try {
			Type <T>::check (v);
		} catch (...) {
			reinterpret_cast <T&> (v).~T (); // Destructor mustn't throw exceptions
			new (&v) T ();
			throw;
		}
	} else
		Type <T>::check (v);
}

// Outline for compact code
template <typename T, typename TABI>
TypeWithCheck <T, TABI>::C_inout::~C_inout () noexcept (false)
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
