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
#ifndef NIRVANA_ORB_TYPECODEOPS_H_
#define NIRVANA_ORB_TYPECODEOPS_H_

#include "Type.h"
#include <new>

namespace CORBA {
namespace Internal {

template <typename T>
class TypeCodeOps
{
public:
	typedef typename Type <T>::ABI ABI;
	typedef typename std::conditional <std::is_same <T, Boolean>::value,
		Type <Boolean>::ABI, typename Type <T>::Var>::type Var;

	static size_t _s_n_aligned_size (Bridge <TypeCode>*, Interface*)
	{
		return sizeof (Var);
	}

	static size_t _s_n_CDR_size (Bridge <TypeCode>*, Interface*)
	{
		return Type <T>::CDR_size;
	}

	static size_t _s_n_align (Bridge <TypeCode>*, Interface*)
	{
		return Type <T>::CDR_align;
	}

	static Type <Boolean>::ABI_ret _s_n_is_CDR (Bridge <TypeCode>*, Interface*)
	{
		return Type <T>::is_CDR;
	}

	static void n_construct (void* p)
	{
		check_pointer (p);
		new (p) Var ();
	}

	static void n_destruct (void* p)
	{
		check_pointer (p);
		reinterpret_cast <Var*> (p)->~Var ();
	}

	static void n_copy (void* dst, const void* src)
	{
		check_pointer (dst);
		check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI*> (src));
		new (dst) Var (*reinterpret_cast <const Var*> (src));
	}

	static void n_move (void* dst, void* src)
	{
		check_pointer (dst);
		check_pointer (src);
		if (Type <T>::has_check)
			Type <T>::check (*reinterpret_cast <const ABI*> (src));
		new (dst) Var (std::move (*reinterpret_cast <Var*> (src)));
	}

	static void n_marshal_in (const void* src, size_t count, IORequest_ptr rq)
	{
		check_pointer (src);
		Type <T>::marshal_in_a (reinterpret_cast <const Var*> (src), count, rq);
	}

	static void n_marshal_out (void* src, size_t count, IORequest_ptr rq)
	{
		check_pointer (src);
		Type <T>::marshal_out_a (reinterpret_cast <Var*> (src), count, rq);
	}

	static void n_unmarshal (IORequest_ptr rq, size_t count, void* dst)
	{
		check_pointer (dst);
		Type <T>::unmarshal_a (rq, count, reinterpret_cast <Var*> (dst));
	}

	static void n_byteswap (void* p, size_t count)
	{
		if (Type <T>::is_CDR) {
			check_pointer (p);
			for (Var* pv = (Var*)p, *end = pv + count; pv != end; ++pv)
				Type <T>::byteswap (*pv);
		}
	}

private:
	static void check (const void* p, size_t count);
};

template <typename T>
void TypeCodeOps <T>::check (const void* p, size_t count)
{
	for (const ABI* pa = reinterpret_cast <const ABI*> (p), *end = pa + count; pa != end; ++pa) {
		Type <T>::check (*pa);
	}
}

template <>
class TypeCodeOps <void>
{
public:
	typedef void Valtype;

	static size_t _s_n_aligned_size (Bridge <TypeCode>*, Interface*)
	{
		return 0;
	}

	static size_t _s_n_CDR_size (Bridge <TypeCode>*, Interface*)
	{
		return 0;
	}

	static size_t _s_n_align (Bridge <TypeCode>*, Interface*)
	{
		return 1;
	}

	static Type <Boolean>::ABI_ret _s_n_is_CDR (Bridge <TypeCode>*, Interface*)
	{
		return true;
	}

	static void _s_n_construct (Bridge <TypeCode>*, void*, Interface*)
	{}

	static void _s_n_destruct (Bridge <TypeCode>*, void*, Interface*)
	{}

	static void _s_n_copy (Bridge <TypeCode>*, void*, const void*, Interface*)
	{}

	static void _s_n_move (Bridge <TypeCode>*, void*, void*, Interface*)
	{}

	static void _s_n_marshal_in (Bridge <TypeCode>*, const void*, size_t,
		Interface*, Interface*)
	{}

	static void _s_n_marshal_out (Bridge <TypeCode>*, void*, size_t,
		Interface*, Interface*)
	{}

	static void _s_n_unmarshal (Bridge <TypeCode>*, Interface*, size_t,
		void*, Interface*)
	{}

	static void _s_n_byteswap (Bridge <TypeCode>*, void*, size_t, Interface*)
	{}
};

}
}

#endif
