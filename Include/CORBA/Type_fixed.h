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
#ifndef NIRVANA_ORB_TYPE_FIXED_H_
#define NIRVANA_ORB_TYPE_FIXED_H_
#pragma once

#include "Fixed.h"
#include "TypeByRef.h"

namespace CORBA {
namespace Internal {

void BCD_check (const Octet* bcd, size_t size);

template <uint16_t digits, int16_t scale>
struct Type <IDL::Fixed <digits, scale> > :
	public TypeByRefCheck <IDL::Fixed <digits, scale> >
{
	static const bool is_CDR = true;
	static const bool has_check = true;
	static const bool is_var_len = false;

	typedef TypeByRefCheck <IDL::Fixed <digits, scale> > Base;
	typedef typename Base::ABI ABI;
	typedef typename Base::Var Var;

	static void check (const Var& var)
	{
		const IDL::FixedBCD <digits, scale>& abi = var;
		BCD_check (abi.bcd, sizeof (abi.bcd));
	}
	
	// Marshaling

	inline static void marshal_in (const Var& src, IORequest_ptr rq);
	inline static void marshal_in_a (const Var* src, size_t count, IORequest_ptr rq);

	static void marshal_out (Var& src, IORequest_ptr rq)
	{
		marshal_in (src, rq);
	}

	static void marshal_out_a (Var* src, size_t count, IORequest_ptr rq)
	{
		marshal_in_a (src, count, rq);
	}

	inline static void unmarshal (IORequest_ptr rq, Var& dst);
	inline static void unmarshal_a (IORequest_ptr rq, size_t count, Var* dst);

	static void byteswap (Var&) noexcept
	{}

	static I_ptr <TypeCode> type_code () noexcept;
};

}
}

#endif
