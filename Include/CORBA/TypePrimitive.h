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
#ifndef NIRVANA_ORB_TYPEPRIMITIVE_H_
#define NIRVANA_ORB_TYPEPRIMITIVE_H_
#pragma once

#include "primitive_types.h"
#include "TypeFixLen.h"
#include <Nirvana/bitutils.h>

namespace CORBA {
namespace Internal {

// Byte order swap

inline UShort byteswap (UShort& v) NIRVANA_NOEXCEPT
{
	return Nirvana::byteswap (v);
}

inline Short byteswap (const Short& v) NIRVANA_NOEXCEPT
{
	return Nirvana::byteswap ((const uint16_t&)v);
}

inline ULong byteswap (const ULong& v) NIRVANA_NOEXCEPT
{
	return Nirvana::byteswap (v);
}

inline Long byteswap (const Long& v) NIRVANA_NOEXCEPT
{
	return Nirvana::byteswap ((const uint32_t&)v);
}

inline LongLong byteswap (const LongLong& v) NIRVANA_NOEXCEPT
{
	return Nirvana::byteswap ((const uint64_t&)v);
}

inline Float byteswap (const Float& v) NIRVANA_NOEXCEPT
{
	Float ret;
	reinterpret_cast <uint32_t&> (ret) = Nirvana::byteswap ((const uint32_t&)v);
	return ret;
}

inline Double byteswap (const Double& v) NIRVANA_NOEXCEPT
{
	Double ret;
	reinterpret_cast <uint64_t&> (ret) = Nirvana::byteswap ((const uint64_t&)v);
	return ret;
}

inline LongDouble byteswap (const LongDouble& v) NIRVANA_NOEXCEPT
{
	const uint64_t* src = &reinterpret_cast <const uint64_t&> (v);
	uint64_t dst [2] = { Nirvana::byteswap (src [1]), Nirvana::byteswap (src [0]) };
	return *(const LongDouble*)dst;
}

inline uint8_t byteswap (const uint8_t& v) NIRVANA_NOEXCEPT
{
	return v;
}

template <typename T>
struct TypePrimitive :
	public TypeFixLen <T, T>
{
	static void byteswap (T& v) NIRVANA_NOEXCEPT
	{
		v = Internal::byteswap (v);
	}
};

template <>
struct TypePrimitive <Char> :
	public TypeByVal <Char>
{
	static void marshal_in (const Char& src, IORequest_ptr rq);
	static void marshal_in_a (const Char* src, size_t count, IORequest_ptr rq);

	static void unmarshal (IORequest_ptr rq, Char& dst);
	static void unmarshal_a (IORequest_ptr rq, size_t count, Char* dst);

};

template <>
struct TypePrimitive <WChar> :
	public TypeByVal <WChar>
{
	static void marshal_in (const WChar& src, IORequest_ptr rq);
	static void marshal_in_a (const WChar* src, size_t count, IORequest_ptr rq);

	static void unmarshal (IORequest_ptr rq, WChar& dst);
	static void unmarshal_a (IORequest_ptr rq, size_t count, WChar* dst);
};

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.

template <>
struct TypePrimitive <Boolean> :
	public TypeByVal <Boolean, Octet>
{
	class C_inout
	{
	public:
		C_inout (Var& v) :
			ref_ (v),
			abi_ (v)
		{}

		~C_inout ()
		{
			ref_ = abi_;
		}

		ABI_out operator & ()
		{
			return (Octet*)&abi_;
		}

	protected:
		Var& ref_;
		// The ABI for boolean is Char.
		// But sizeof(bool) is implementation-dependent and might be > sizeof(Char).
		// So we reserve size_t (the machine word) as ABI for boolean in assumption that bool implementation can't be wide.
		size_t abi_;
	};

	typedef C_inout C_out;

	static void marshal_in (const ABI& src, IORequest_ptr rq);
	static void marshal_in_a (const ABI* src, size_t count, IORequest_ptr rq);

	static void marshal_in (const Boolean src, IORequest_ptr rq)
	{
		ABI abi = src;
		marshal_in (abi, rq);
	}

	static void marshal_out (ABI& src, IORequest_ptr rq)
	{
		marshal_in (src, rq);
	}

	static void marshal_out_a (ABI* src, size_t count, IORequest_ptr rq)
	{
		marshal_in_a (src, count, rq);
	}

	static void marshal_out (Boolean src, IORequest_ptr rq)
	{
		marshal_in (src, rq);
	}

	static void unmarshal (IORequest_ptr rq, ABI& dst);
	static void unmarshal_a (IORequest_ptr rq, size_t count, ABI* dst);

	static void unmarshal (IORequest_ptr rq, Boolean& dst)
	{
		ABI abi;
		unmarshal (rq, abi);
		dst = abi != 0;
	}
};

#ifdef LEGACY_CORBA_CPP
typedef Boolean& Boolean_out;
#endif

}
}

#endif
