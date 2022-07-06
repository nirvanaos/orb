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
#ifndef NIRVANA_ORB_TYPEENUM_H_
#define NIRVANA_ORB_TYPEENUM_H_
#pragma once

#include <Nirvana/throw_exception.h>
#include "TypeByVal.h"

namespace CORBA {
namespace Internal {

/// Base for enum data types
template <class T, T last>
struct TypeEnum : TypeByVal <T, ABI_enum>
{
	static_assert (sizeof (T) == sizeof (ABI_enum), "IDL enumerations must be declared as : ABI_enum.");

	static const ABI_enum count_ = (ABI_enum)last + 1;

	typedef TypeByVal <T, ABI_enum> Base;

	typedef typename Base::Var Var;
	typedef typename Base::ABI ABI;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_ret ABI_ret;

	static const bool has_check = true;
	static const bool is_CDR = true;

	static void check (ABI val)
	{
		if (val >= count_)
			::Nirvana::throw_BAD_PARAM (MAKE_OMG_MINOR (25));
	}

	static void check (T val)
	{
		check ((ABI)val);
	}

	class C_inout
	{
	public:
		C_inout (Var& val) :
			ref_ (val),
			val_ ((ABI)val)
		{}

		~C_inout () noexcept (false)
		{
			if (!uncaught_exception ()) {
				Type <Var>::check (val_);
				ref_ = (T*)val_;
			}
		}

		ABI* operator & ()
		{
			return &val_;
		}

	private:
		Var& ref_;
		ABI val_;
	};

	typedef C_inout C_out;

	class C_ret
	{
	public:
		C_ret (ABI_ret val) :
			val_ (val)
		{
			check (val_);
		}

		operator Var ()
		{
			return (Var)val_;
		}

	private:
		ABI_ret val_;
	};

	static Var in (ABI_in v)
	{
		check (v);
		return (Var)v;
	}

	static Var& inout (ABI_out p)
	{
		check_pointer (p);
		check (*p);
		return (Var&)*p;
	}

	static Var& out (ABI_out p)
	{
		check_pointer (p);
		return (Var&)*p;
	}

	static void marshal_in (const T& src, IORequest_ptr rq);
	static void marshal_in_a (const T* src, size_t count, IORequest_ptr rq);

	static void unmarshal (IORequest_ptr rq, T& dst);
	static void unmarshal_a (IORequest_ptr rq, size_t count, T* dst);

	static void byteswap (Var& v) NIRVANA_NOEXCEPT
	{
		Type <ABI>::byteswap ((ABI&)v);
	}
};

}
}

#endif
