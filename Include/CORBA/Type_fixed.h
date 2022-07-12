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

#include <Nirvana/Decimal.h>
#include "TypeFixLen.h"

namespace IDL {

template <uint16_t digits, int16_t scale>
using Fixed = Nirvana::Decimal <digits, scale>;

}

namespace CORBA {
namespace Internal {

void check_BCD (const Octet* bcd, size_t size);

template <uint16_t digits, int16_t scale>
struct Type <IDL::Fixed <digits, scale> > :
	public TypeFixLen <IDL::Fixed <digits, scale>, true, IDL::FixedBCD <digits, scale> >
{
	static const bool is_CDR = true;
	static const bool has_check = true;

	typedef TypeFixLen <IDL::Fixed <digits, scale>, true, IDL::FixedBCD <digits, scale> > Base;
	typedef typename Base::ABI ABI;
	typedef typename Base::Var Var;

	typedef const IDL::FixedBCD <digits, scale>& C_in;

	static void check (const ABI& abi)
	{
		check_BCD (abi.bcd, sizeof (abi.bcd));
	}
	
	static void byteswap (Var&) NIRVANA_NOEXCEPT
	{}

	static I_ptr <TypeCode> type_code () NIRVANA_NOEXCEPT;
};

}
}

#endif
