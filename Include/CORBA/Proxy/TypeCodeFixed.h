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

template <UShort digits, Short scale>
class TypeCodeFixed :
	public TypeCodeStatic <TypeCodeFixed <digits, scale>,
		TypeCodeTK <TCKind::tk_fixed>, TypeCodeOps <IDL::Fixed <digits, scale> > >
{
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
};

template <uint16_t digits, int16_t scale> inline
I_ptr <TypeCode> Type <IDL::Fixed <digits, scale> >::type_code () NIRVANA_NOEXCEPT
{
	return TypeCodeFixed <digits, scale>::_get_ptr ();
}

}
}

#endif
