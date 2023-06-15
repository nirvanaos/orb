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
#ifndef NIRVANA_ORB_TYPEFIXLEN_INL_
#define NIRVANA_ORB_TYPEFIXLEN_INL_
#pragma once

#include "TypeFixLen.h"
#include "IORequestClient.h"
#include <Nirvana/real_copy.h>

namespace CORBA {
namespace Internal {

template <typename T, bool chk, typename TABI> inline
void TypeFixLen <T, chk, TABI>::marshal_in (const T& src, IORequest_ptr rq)
{
	rq->marshal (CDR_align, CDR_size, &src);
}

template <typename T, bool chk, typename TABI> inline
void TypeFixLen <T, chk, TABI>::marshal_in_a (const T* src, size_t count, IORequest_ptr rq)
{
	assert (count);
	if (sizeof (T) == CDR_size)
		rq->marshal (CDR_align, count * CDR_size, src);
	else {
		size_t zero = 0;
		rq->marshal_seq (CDR_align, sizeof (T), CDR_size, count, const_cast <T*> (src), zero);
	}
}

template <typename T, bool chk, typename TABI> inline
void TypeFixLen <T, chk, TABI>::unmarshal (IORequest_ptr rq, T& dst)
{
	if (rq->unmarshal (CDR_align, CDR_size, &dst))
		Type <T>::byteswap (dst);
	if (Type <T>::has_check)
		Type <T>::check ((const typename Type <T>::ABI&)dst);
}

template <typename T, bool chk, typename TABI> inline
void TypeFixLen <T, chk, TABI>::unmarshal_a (IORequest_ptr rq, size_t count, T* dst)
{
	assert (count);
	bool byte_swap;
	if (sizeof (T) == CDR_size)
		byte_swap = rq->unmarshal (CDR_align, count * CDR_size, dst);
	else {
		size_t size = sizeof (T) * count;
		void* p = dst;
		byte_swap = rq->unmarshal_seq (CDR_align, sizeof (T), CDR_size, count, p, size);
		assert (p == dst); // Pointer must not be allocated by request
	}
	if (sizeof (T) > 1 && byte_swap) {
		for (T* p = dst, *end = dst + count; p != end; ++p) {
			Type <T>::byteswap (*p);
		}
	}

	if (Type <T>::has_check) {
		for (T* p = dst, *end = dst + count; p != end; ++p) {
			Type <T>::check (*(const typename Type <T>::ABI*)p);
		}
	}
}

}
}

#endif
