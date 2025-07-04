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
#ifndef NIRVANA_ORB_TYPE_ARRAY_H_
#define NIRVANA_ORB_TYPE_ARRAY_H_
#pragma once

#include "TypeFixLen.h"
#include "TypeVarLen.h"
#include "ABI.h"
#include <Nirvana/array_bool.h>

// Note that reference types are mapped to interface types.
// So Type <I_ref <Itf> > provide the same traits as Type <Itf>.

namespace CORBA {

class TypeCode;

namespace Internal {

template <class T>
struct ArrayTraits
{
  typedef T ElType;
  static const size_t size = 1;
	static const uint32_t dimensions = 0;
};

template <class T>
const size_t ArrayTraits <T>::size;

template <class T>
const uint32_t ArrayTraits <T>::dimensions;

template <class T, size_t bound>
struct ArrayTraits <std::array <T, bound> >
{
  typedef typename ArrayTraits <T>::ElType ElType;
  static const size_t size = ArrayTraits <T>::size * bound;
	static const uint32_t dimensions = ArrayTraits <T>::dimensions + 1;
};

template <class T, size_t bound>
const size_t ArrayTraits <std::array <T, bound> >::size;

template <class T, size_t bound>
const uint32_t ArrayTraits <std::array <T, bound> >::dimensions;

template <class T, size_t bound>
using TypeArrayBase = typename std::conditional <Type <typename ArrayTraits <T>::ElType>::is_var_len,
	TypeVarLen <std::array <T, bound>, std::array <T, bound> >,
	TypeFixLen <std::array <T, bound>, Type <typename ArrayTraits <T>::ElType>::has_check>
>::type;

template <class T, size_t bound>
struct Type <std::array <T, bound> > :
	public TypeArrayBase <T, bound>
{
	typedef TypeArrayBase <T, bound> Base;

	typedef typename Base::Var Var;
	typedef typename ArrayTraits <Var>::ElType ET;
	typedef typename Base::ABI ABI;
	typedef typename Type <ET>::ABI ET_ABI;
	typedef typename Type <ET>::Var ET_Var;

	static const size_t total_size = ArrayTraits <Var>::size;
	static const uint32_t dimensions = ArrayTraits <Var>::dimensions;
	static const bool is_CDR = Type <ET>::is_CDR;
	static const bool has_check = Type <ET>::has_check;
	static const size_t CDR_align = Type <ET>::CDR_align;
	static const size_t CDR_size = (total_size - 1) * sizeof (ET_ABI) + Type <ET>::CDR_size;

	static void check (const ABI& abi)
	{
		if (has_check) {
			const ET_ABI* p = reinterpret_cast <const ET_ABI*> (abi.data ()), * end = p + total_size;
			do {
				Type <ET>::check (*(p++));
			} while (p != end);
		}
	}

	static I_ptr <TypeCode> type_code () noexcept;

	static void marshal_in_a (const Var* src, size_t count, IORequest_ptr rq)
	{
		Type <ET>::marshal_in_a (reinterpret_cast <const ET_Var*> (src->data ()), total_size * count, rq);
	}

	static void marshal_in (const Var& src, IORequest_ptr rq)
	{
		Type <ET>::marshal_in_a (reinterpret_cast <const ET_Var*> (src.data ()), total_size, rq);
	}

	static void marshal_out_a (Var* src, size_t count, IORequest_ptr rq)
	{
		Type <ET>::marshal_out_a (reinterpret_cast <ET_Var*> (src->data ()), total_size * count, rq);
	}

	static void marshal_out (Var& src, IORequest_ptr rq)
	{
		Type <ET>::marshal_out_a (reinterpret_cast <ET_Var*> (src.data ()), total_size, rq);
	}

	static void unmarshal_a (IORequest_ptr rq, size_t count, Var* dst)
	{
		Type <ET>::unmarshal_a (rq, total_size * count, reinterpret_cast <ET_Var*> (dst->data ()));
	}

	static void unmarshal (IORequest_ptr rq, Var& dst)
	{
		Type <ET>::unmarshal_a (rq, total_size, reinterpret_cast <ET_Var*> (dst.data ()));
	}

	static void byteswap (Var& var)
	{
		ET_Var* p = reinterpret_cast <ET_Var*> (var.data ()), *end = p + total_size;
		do {
			Type <ET>::byteswap (*(p++));
		} while (p != end);
	}
};

template <class T, size_t bound>
const size_t Type <std::array <T, bound> >::total_size;

template <class T, size_t bound>
const uint32_t Type <std::array <T, bound> >::dimensions;

template <class T, size_t bound>
const bool Type <std::array <T, bound> >::is_CDR;

template <class T, size_t bound>
const bool Type <std::array <T, bound> >::has_check;

template <class T, size_t bound>
const size_t Type <std::array <T, bound> >::CDR_align;

template <class T, size_t bound>
const size_t Type <std::array <T, bound> >::CDR_size;

}
}

namespace IDL {

template <class T, size_t bound>
struct traits <std::array <T, bound> > : CORBA::Internal::Traits <std::array <T, bound> >
{
	using element_traits = traits <typename Type <std::array <T, bound> >::ET>;
	using dimensions = std::integral_constant <uint32_t, Type <std::array <T, bound> >::dimensions>;
};

}

#endif
