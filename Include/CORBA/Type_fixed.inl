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
#ifndef NIRVANA_ORB_TYPE_FIXED_INL_
#define NIRVANA_ORB_TYPE_FIXED_INL_
#pragma once

#include "Type_fixed.h"
#include "IORequestClient.h"

namespace CORBA {
namespace Internal {

template <uint16_t digits, int16_t scale> inline
void Type <IDL::Fixed <digits, scale> >::marshal_in (const Var& src, IORequest_ptr rq)
{
	rq->marshal (1, sizeof (Var), &src);
}

template <uint16_t digits, int16_t scale> inline
void Type <IDL::Fixed <digits, scale> >::marshal_in_a (const Var* src, size_t count, IORequest_ptr rq)
{
	rq->marshal (1, sizeof (Var) * count, src);
}

template <uint16_t digits, int16_t scale> inline
void Type <IDL::Fixed <digits, scale> >::unmarshal (IORequest_ptr rq, Var& dst)
{
	rq->unmarshal (1, sizeof (Var), &dst);
#ifndef NDEBUG
	check (dst);
#endif
}

template <uint16_t digits, int16_t scale> inline
void Type <IDL::Fixed <digits, scale> >::unmarshal_a (IORequest_ptr rq, size_t count, Var* dst)
{
	rq->unmarshal (1, sizeof (Var) * count, dst);
#ifndef NDEBUG
	for (Var* p = dst, *end = p + count; p != end; ++p) {
		check (*p);
	}
#endif
}

}
}

#endif
