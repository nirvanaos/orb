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

inline UShort byteswap (UShort& v)
{
	return Nirvana::byteswap (v);
}

inline Short byteswap (const Short& v)
{
	return Nirvana::byteswap ((const uint16_t&)v);
}

inline ULong byteswap (const ULong& v)
{
	return Nirvana::byteswap (v);
}

inline Long byteswap (const Long& v)
{
	return Nirvana::byteswap ((const uint32_t&)v);
}

inline LongLong byteswap (const LongLong& v)
{
	return Nirvana::byteswap ((const uint64_t&)v);
}

inline Float byteswap (const Float& v)
{
	Float ret;
	reinterpret_cast <uint32_t&> (ret) = Nirvana::byteswap ((const uint32_t&)v);
	return ret;
}

inline Double byteswap (const Double& v)
{
	Double ret;
	reinterpret_cast <uint64_t&> (ret) = Nirvana::byteswap ((const uint64_t&)v);
	return ret;
}

inline LongDouble byteswap (const LongDouble& v)
{
	const uint64_t* src = &reinterpret_cast <const uint64_t&> (v);
	uint64_t dst [2] = { Nirvana::byteswap (src [1]), Nirvana::byteswap (src [0]) };
	return *(const LongDouble*)dst;
}

inline uint8_t byteswap (const uint8_t& v)
{
	return v;
}

template <typename T>
struct TypePrimitive :
	public TypeFixLen <T, T>
{
	static void byteswap (T& v)
	{
		v = Internal::byteswap (v);
	}
};

template <>
struct TypePrimitive <Char> :
	public TypeByVal <Char>
{
	static const bool is_CDR = false;

	static void marshal_in (const Char* src, size_t count, IORequest::_ptr_type rq)
	{
		rq->marshal_char (count, src);
	}

	static void marshal_out (Char* src, size_t count, IORequest::_ptr_type rq)
	{
		marshal_in (src, count, rq);
	}

	static void unmarshal (IORequest::_ptr_type rq, size_t count, Char* dst)
	{
		rq->unmarshal_char (count, dst);
	}
};

template <>
struct TypePrimitive <WChar> :
	public TypeByVal <WChar>
{
	static const bool is_CDR = false;

	static void marshal_in (const WChar* src, size_t count, IORequest::_ptr_type rq)
	{
		rq->marshal_wchar (count, src);
	}

	static void marshal_out (WChar* src, size_t count, IORequest::_ptr_type rq)
	{
		marshal_in (src, count, rq);
	}

	static void unmarshal (IORequest::_ptr_type rq, size_t count, WChar* dst)
	{
		rq->unmarshal_wchar (count, dst);
	}
};

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.

template <>
struct TypePrimitive <Boolean> :
	public TypeByVal <Boolean, Char>
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
			return (Char*)&abi_;
		}

	protected:
		Var& ref_;
		// The ABI for boolean is Char.
		// But sizeof(bool) is implementation-dependent and might be > sizeof(Char).
		// So we reserve size_t (the machine word) as ABI for boolean in assumption that bool implementation can't be wide.
		size_t abi_;
	};

	typedef C_inout C_out;

	// We have 2 kinds of marshal unmarshal methods.

	static void marshal_in (const ABI* src, size_t count, IORequest::_ptr_type rq)
	{
		rq->marshal (1, count, src);
	}

	static void marshal_in (const Boolean* src, size_t count, IORequest::_ptr_type rq)
	{
		assert (1 == count);
		ABI* buf = (ABI*)rq->marshal_get_buffer (alignof (ABI), sizeof (ABI) * count);
		for (const Boolean* end = src + count; src != end; ++src)
			*(buf++) = src;
	}

	static void marshal_out (ABI* src, size_t count, IORequest::_ptr_type rq)
	{
		marshal_in (src, count, rq);
	}

	static void marshal_out (Boolean* src, size_t count, IORequest::_ptr_type rq)
	{
		marshal_in (src, count, rq);
	}

	static void unmarshal (IORequest::_ptr_type rq, size_t count, ABI* dst)
	{
		void* pbuf = nullptr;
		rq->unmarshal (1, count, pbuf);
		real_copy ((const ABI*)pbuf, (const ABI*)pbuf + count, dst);
	}

	static void unmarshal (IORequest::_ptr_type rq, size_t count, Boolean* dst)
	{
		assert (1 == count);
		void* pbuf = nullptr;
		rq->unmarshal (1, count, pbuf);
		for (const ABI* src = (const ABI*)pbuf,* end = src + count; src != end; ++src)
			*(dst++) = src != 0;
	}
};

#ifdef LEGACY_CORBA_CPP
typedef Boolean& Boolean_out;
#endif

}
}

#endif
