/// \file TypeCodeImpl.h
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
#ifndef NIRVANA_ORB_TYPECODEFIXED_H_
#define NIRVANA_ORB_TYPECODEFIXED_H_
#pragma once

#include "TypeCodeImpl.h"
#include "../Type_fixed.h"

namespace CORBA {
namespace Internal {

template <size_t size>
class FixedOps
{
public:
	static size_t _s_n_size (Bridge <TypeCode>*, Interface*)
	{
		return size;
	}

	static Type <Boolean>::ABI_ret _s_n_is_CDR (Bridge <TypeCode>*, Interface*)
	{
		return true;
	}

	static void _s_n_construct (Bridge <TypeCode>*, void* p, Interface*)
	{
		check_pointer (p);
		Nirvana::BCD_zero ((Octet*)p, size);
	}

	static void _s_n_destruct (Bridge <TypeCode>*, void* p, Interface*)
	{}

	static void n_copy (void* dst, const void* src);

	static void n_move (void* dst, void* src)
	{
		n_copy (dst, src);
	}

	static void n_marshal_in (const void* src, size_t count, IORequest_ptr rq);

	static void n_marshal_out (void* src, size_t count, IORequest_ptr rq)
	{
		n_marshal_in (src, count, rq);
	}

	static void n_unmarshal (IORequest_ptr rq, size_t count, void* dst)
	{
		check_pointer (dst);
		void* data;
		rq->unmarshal (1, size, data);
		BCD_check ((const Octet*)data, size);
		Nirvana::real_copy ((const Octet*)data, (const Octet*)data + size, (Octet*)dst);
	}
};

template <size_t size>
void FixedOps <size>::n_copy (void* dst, const void* src)
{
	check_pointer (dst);
	check_pointer (src);
	BCD_check ((const Octet*)src, size);
	Nirvana::real_copy ((const Octet*)src, (const Octet*)src + size, (Octet*)dst);
}

template <size_t size>
void FixedOps <size>::n_marshal_in (const void* src, size_t count, IORequest_ptr rq)
{
	check_pointer (src);
	for (const Octet* p = (const Octet*)src; count; p += size, --count) {
		BCD_check (p, size);
	}
	rq->marshal (1, size, src);
}

template <UShort digits, Short scale>
class TypeCodeFixed :
	public TypeCodeStatic <TypeCodeFixed <digits, scale>,
		TypeCodeTK <TCKind::tk_fixed>, FixedOps <sizeof (IDL::FixedBCD <digits, scale>)> >
{
	typedef FixedOps <sizeof (IDL::FixedBCD <digits, scale>)> Ops;
public:
	static UShort _s_fixed_digits (Bridge <TypeCode>* _b, Interface* _env)
	{
		return digits;
	}

	static Short _s_fixed_scale (Bridge <TypeCode>* _b, Interface* _env)
	{
		return scale;
	}

	static Boolean equal (I_ptr <TypeCode> other)
	{
		return TypeCodeBase::equal (TCKind::tk_fixed, other)
			&& digits == other->fixed_digits ()
			&& scale == other->fixed_scale ();
	}

	static Boolean equivalent (I_ptr <TypeCode> other)
	{
		return equal (other);
	}

	using Ops::_s_n_construct;
	using Ops::_s_n_destruct;
};

template <uint16_t digits, int16_t scale> inline
I_ptr <TypeCode> Type <IDL::Fixed <digits, scale> >::type_code () NIRVANA_NOEXCEPT
{
	return TypeCodeFixed <digits, scale>::_get_ptr ();
}

}
}

#endif
