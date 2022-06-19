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
#include <Nirvana/array_bool.h>

namespace CORBA {
namespace Internal {

template <class T>
struct ArrayTraits
{
  typedef T ElType;
  static const size_t size = 1;
};

template <class T, size_t S>
struct ArrayTraits <std::array <T, S> >
{
  typedef typename ArrayTraits <T>::ElType ElType;
  static const size_t size = ArrayTraits <T>::size * S;
};

template <class AT, size_t AS>
using TypeArrayBase = typename std::conditional <Type <typename ArrayTraits <AT>::ElType>::fixed_len,
	TypeFixLen <std::array <AT, AS>, std::array <AT, AS> >,
	TypeVarLen <std::array <AT, AS>, Type <typename ArrayTraits <AT>::ElType>::has_check, std::array <AT, AS> >
>::type;

template <class AT, size_t AS>
struct Type <std::array <AT, AS> > : public 
	std::conditional <Type <typename ArrayTraits <AT>::ElType>::fixed_len,
	TypeFixLen <std::array <AT, AS>, std::array <AT, AS> >,
	TypeVarLen <std::array <AT, AS>, Type <typename ArrayTraits <AT>::ElType>::has_check, std::array <AT, AS> >
	>::type
{
	typedef typename std::conditional <Type <typename ArrayTraits <AT>::ElType>::fixed_len,
		TypeFixLen <std::array <AT, AS>, std::array <AT, AS> >,
		TypeVarLen <std::array <AT, AS>, Type <typename ArrayTraits <AT>::ElType>::has_check, std::array <AT, AS> >
	>::type Base;

	typedef typename Base::Var Var;
	typedef typename ArrayTraits <Var>::ElType VT;
	typedef typename Base::ABI ABI;

	static const size_t total_size = ArrayTraits <Var>::size;
	static const bool fixed_len = Type <VT>::fixed_len;
	static const bool has_check = Type <VT>::has_check;

	static void check (const ABI& abi)
	{
		if (has_check) {
			const VT* p = reinterpret_cast <const VT*> (abi.data ()), * end = p + total_size;
			do {
				Type <VT>::check (*(p++));
			} while (p != end);
		}
	}

	static void marshal_in_a (const Var* src, size_t count, IORequest_ptr rq)
	{
		Type <VT>::marshal_in_a (reinterpret_cast <const VT*> (src->data ()), total_size * count, rq);
	}

	static void marshal_in (const Var& src, IORequest_ptr rq)
	{
		Type <VT>::marshal_in_a (reinterpret_cast <const VT*> (src.data ()), total_size, rq);
	}

	static void marshal_out_a (Var* src, size_t count, IORequest_ptr rq)
	{
		Type <VT>::marshal_out_a (reinterpret_cast <VT*> (src->data ()), total_size * count, rq);
	}

	static void marshal_out (Var& src, IORequest_ptr rq)
	{
		Type <VT>::marshal_out_a (reinterpret_cast <VT*> (src->data ()), total_size, rq);
	}

	static void unmarshal_a (IORequest_ptr rq, size_t count, Var* dst)
	{
		Type <VT>::unmarshal_a (rq, total_size * count, reinterpret_cast <VT*> (dst->data ()));
	}

	static void unmarshal (IORequest_ptr rq, Var& dst)
	{
		Type <VT>::unmarshal_a (rq, total_size, *reinterpret_cast <VT*> (dst->data ()));
	}

	static void byteswap (Var& var)
	{
		VT* p = var.data (), *end = p + total_size;
		do {
			Type <VT>::byteswap (*(p++));
		} while (p != end);
	}
};

}
}

#endif
