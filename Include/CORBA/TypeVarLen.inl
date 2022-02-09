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
#ifndef NIRVANA_ORB_TYPEVARLEN_INL_
#define NIRVANA_ORB_TYPEVARLEN_INL_
#pragma once

#include "TypeVarLen.h"
#include "IORequestClient.h"

namespace CORBA {
namespace Internal {

template <class T, class TABI>
template <typename MT> inline
void TypeVarLenBase <T, TABI>::marshal_members (const MT* begin, const void* end, IORequest_ptr rq)
{
	rq->marshal (alignof (MT), (const Octet*)end - (const Octet*)begin, begin);
}

template <class T, class TABI>
template <typename MT> inline
void TypeVarLenBase <T, TABI>::marshal_members (const Var& val, const MT* begin, IORequest_ptr rq)
{
	rq->marshal (alignof (MT), (const Octet*)(&val + 1) - (const Octet*)begin, begin);
}

template <class T, class TABI>
template <typename MT> inline
bool TypeVarLenBase <T, TABI>::unmarshal_members (IORequest_ptr rq, MT* begin, const void* end)
{
	size_t size = (const Octet*)&end - (const Octet*)&begin;
	void* buf;
	bool swap_bytes = rq->unmarshal (alignof (MT), size, buf);
	Nirvana::real_copy ((const Octet*)buf, (const Octet*)buf + size, (Octet*)begin);
	return swap_bytes;
}

template <class T, class TABI>
template <typename MT> inline
bool TypeVarLenBase <T, TABI>::unmarshal_members (IORequest_ptr rq, const Var& val, MT* begin)
{
	size_t size = (const Octet*)(&val + 1) - (const Octet*)begin;
	void* buf;
	bool swap_bytes = rq->unmarshal (alignof (MT), size, buf);
	Nirvana::real_copy ((const Octet*)buf, (const Octet*)buf + size, (Octet*)begin);
	return swap_bytes;
}

}
}

#endif
