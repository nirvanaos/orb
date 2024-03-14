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
struct TypeEnum : TypeByValCheck <T, ABI_enum>
{
	static_assert (sizeof (T) == sizeof (ABI_enum), "IDL enumerations must be declared as : ABI_enum.");

	static const ABI_enum count_ = (ABI_enum)last + 1;

	typedef TypeByVal <T, ABI_enum> Base;

	typedef typename Base::Var Var;
	typedef typename Base::ABI ABI;
	typedef typename Base::ABI_in ABI_in;
	typedef typename Base::ABI_out ABI_out;
	typedef typename Base::ABI_ret ABI_ret;

	static const bool is_CDR = true;
	static const bool is_CDR_seq = true;

	static void check (ABI val)
	{
		if (val >= count_)
			::Nirvana::throw_BAD_PARAM (MAKE_OMG_MINOR (25));
	}

	static void check (T val)
	{
		check ((ABI)val);
	}

	inline static void marshal_in (const T& src, IORequest_ptr rq);
	inline static void marshal_in_a (const T* src, size_t count, IORequest_ptr rq);

	inline static void unmarshal (IORequest_ptr rq, T& dst);
	inline static void unmarshal_a (IORequest_ptr rq, size_t count, T* dst);

	static void byteswap (Var& v) noexcept
	{
		Type <ABI>::byteswap ((ABI&)v);
	}

	class C_VT_ret
	{
	public:
		C_VT_ret (ABI_enum abi) :
			abi_ (abi)
		{}

		operator T () noexcept
		{
			Type <T>::check (abi_);
			return static_cast <T> (abi_);
		}

	protected:
		ABI_enum abi_;
	};
};

}
}

#endif
