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
#ifndef NIRVANA_ORB_TYPEENUM_INL_
#define NIRVANA_ORB_TYPEENUM_INL_
#pragma once

#include "TypeEnum.h"
#include "IORequestClient.h"

namespace CORBA {
namespace Internal {

template <class T, T last> inline
void TypeEnum <T, last>::marshal_in (const T& src, IORequest_ptr rq)
{
	rq->marshal (alignof (T), sizeof (T), &src);
}

template <class T, T last> inline
void TypeEnum <T, last>::marshal_in_a (const T* src, size_t count, IORequest_ptr rq)
{
	rq->marshal (alignof (T), sizeof (T) * count, src);
}

template <class T, T last> inline
void TypeEnum <T, last>::unmarshal (IORequest_ptr rq, T& dst)
{
	if (rq->unmarshal (alignof (T), sizeof (T), &dst))
		byteswap (dst);
	check (dst);
}

template <class T, T last> inline
void TypeEnum <T, last>::unmarshal_a (IORequest_ptr rq, size_t count, T* dst)
{
	if (rq->unmarshal (alignof (T), sizeof (T) * count, dst)) {
		for (T* p = dst, *end = p + count; p != end; ++p) {
			byteswap (*p);
		}
	}
	for (T* p = dst, *end = p + count; p != end; ++p) {
		check (*p);
	}
}

}
}

#endif
