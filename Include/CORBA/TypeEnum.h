/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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

#include "basic_types.h"
#include <Nirvana/throw_exception.h>

namespace CORBA {
namespace Nirvana {

typedef ULong ABI_enum;

/// Base for enum data types
template <class T, T last>
struct TypeEnum
{
	static_assert (sizeof (T) == sizeof (ABI_enum), "IDL enumerations must be declared as 32-bit.");
	static const bool has_check = true;
	static const ABI_enum count_ = (ABI_enum)last + 1;

	typedef ABI_enum ABI_type;

	typedef ABI_type ABI_in;
	typedef ABI_type* ABI_out;
	typedef ABI_type* ABI_inout;
	typedef ABI_type ABI_ret;

	static void check (ABI_enum val)
	{
		if (val >= count_)
			::Nirvana::throw_BAD_PARAM ();
	}

	class C_in
	{
	public:
		C_in (T val) :
			val_ (val)
		{}

		ABI_in operator & () const
		{
			return (ABI_type)val_;
		}

	private:
		T val_;
	};

	class C_inout
	{
	public:
		C_inout (T& val) :
			ref_ (val),
			val_ ((ABI_type)val)
		{}

		~C_inout () noexcept (false)
		{
			if (!uncaught_exception ()) {
				Type <T>::check (val_);
				ref_ = (T*)val_;
			}
		}

		ABI_type* operator & ()
		{
			return &val_;
		}

	private:
		T& ref_;
		ABI_type val_;
	};

	typedef C_inout C_out;

	class C_ret
	{
	public:
		C_ret (ABI_ret val) :
			val_ (val)
		{
			Type <T>::check (val_);
		}

		operator T ()
		{
			return (T)val_;
		}

	private:
		ABI_ret val_;
	};

	static T in (ABI_in v)
	{
		Type <T>::check (v);
		return (T)v;
	}

	static T& inout (ABI_inout p)
	{
		_check_pointer (p);
		Type <T>::check (*p);
		return (T&)*p;
	}

	static T& out (ABI_out p)
	{
		_check_pointer (p);
		return (T&)*p;
	}

	static ABI_ret ret (T val)
	{
		return (ABI_ret)val;
	}
};

}
}

#endif
