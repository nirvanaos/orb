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
#include "ABI.h"
#include "I_ptr.h"
#include <utility>

namespace std {
#ifdef NIRVANA_C17
int uncaught_exceptions () noexcept;
#else
bool uncaught_exception () noexcept;
#endif
}

namespace CORBA {
namespace Internal {

// Helper functions.

extern void check_pointer (const void* p);

inline bool uncaught_exception () noexcept
{
#ifdef NIRVANA_C17
	return std::uncaught_exceptions () != 0;
#else
	return std::uncaught_exception ();
#endif
}

// Forward definitions for interoperable request
class IORequest;
typedef I_ptr <IORequest> IORequest_ptr;

/// Type that does not have check()
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI>
struct TypeNoCheck
{
	static void check (const TABI&) {}
	static const bool has_check = false;

	// Servant-side methods

	static T& out (TABI* p)
	{
		return inout (p);
	}

	static T& inout (TABI* p)
	{
		check_pointer (p);
		return reinterpret_cast <T&> (*p);
	}

};

/// Type that has non-trivial check()
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI>
struct TypeWithCheck
{
	static const bool has_check = true;

	static void check_or_clear (TABI& v);

	class C_inout
	{
	public:
		C_inout (T& val) noexcept :
			ref_ (reinterpret_cast <TABI&> (val))
		{}

		~C_inout () noexcept (false);

		TABI* operator & () const noexcept
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

		operator T () noexcept
		{
			check_or_clear (val_);
			return std::move (reinterpret_cast <T&> (val_));
		}

	protected:
		TABI val_;
	};

	// Servant-side methods

	static T& out (TABI* p)
	{
		return inout (p);
	}

	static T& inout (TABI* p)
	{
		check_pointer (p);
		Type <T>::check (*p);
		return reinterpret_cast <T&> (*p);
	}

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
