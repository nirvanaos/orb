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

#include <Nirvana/throw_exception.h>
#include "TypeFixLen.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

typedef std::conditional_t <sizeof (size_t) >= 4, ULong, size_t> ABI_enum;

/// Base for enum data types
template <class T, T last>
struct TypeEnum : TypeFixLen <T, ABI_enum, T>
{
	static_assert (sizeof (T) == sizeof (ABI_enum), "IDL enumerations must be declared as : ABI_enum.");

	static const ABI_enum count_ = (ABI_enum)last + 1;

	typedef TypeFixLen <T, ABI_enum, T> Base;

	typedef typename Base::Var_type Var_type;
	typedef typename Base::ABI_type ABI_type;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_inout ABI_inout;
	typedef typename Base::ABI_ret ABI_ret;

	static const bool has_check = true;

	static void check (ABI_type val)
	{
		if (val >= count_)
			::Nirvana::throw_BAD_PARAM ();
	}

	class C_inout
	{
	public:
		C_inout (Var_type& val) :
			ref_ (val),
			val_ ((ABI_type)val)
		{}

		~C_inout () noexcept (false)
		{
			if (!uncaught_exception ()) {
				Type <Var_type>::check (val_);
				ref_ = (T*)val_;
			}
		}

		ABI_type* operator & ()
		{
			return &val_;
		}

	private:
		Var_type& ref_;
		ABI_type val_;
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

		operator Var_type ()
		{
			return (Var_type)val_;
		}

	private:
		ABI_ret val_;
	};

	static Var_type in (ABI_in v)
	{
		check (v);
		return (Var_type)v;
	}

	static Var_type& inout (ABI_inout p)
	{
		_check_pointer (p);
		check (*p);
		return (Var_type&)*p;
	}

	static T& out (ABI_out p)
	{
		_check_pointer (p);
		return (Var_type&)*p;
	}

	static ABI_ret ret (Var_type val)
	{
		return (ABI_ret)val;
	}
};

}
}

#endif
