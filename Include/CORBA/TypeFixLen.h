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
#ifndef NIRVANA_ORB_TYPEFIXLEN_H_
#define NIRVANA_ORB_TYPEFIXLEN_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/real_copy.h>
#include "TypeByVal.h"
#include "TypeByRef.h"
#include "Client.h"
#include "TypeCode.h"
#include <type_traits>

namespace CORBA {
namespace Internal {

void marshal_CDR_stub ()
{
	assert (false);
}

/// Fixed-length data types.
/// Passed by value if sizeof (T) <= 2 * sizeof (size_t).
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
/// \tparam TCDR The CDR type.
template <typename T, typename TABI = T, typename TCDR = T>
struct TypeFixLen : std::conditional_t <sizeof (T) <= 2 * sizeof (size_t), TypeByVal <T, TABI>, TypeByRef <T, TABI> >
{
	typedef TCDR CDR;

	static const bool is_CDR = std::is_same <T, TCDR>;

	static void marshal_in (const T* src, size_t count, IORequest::_ptr_type rq)
	{
		if (is_CDR)
			rq->marshal_CDR (alignof (T), sizeof (T) * count, src);
		else
			rq->marshal_fixed_len (alignof (T), sizeof (T), count, src, Type <T>::type_code ());
	}

	static void marshal_out (T* src, size_t count, IORequest::_ptr_type rq)
	{
		marshal_in (src, count, rq);
	}

	static void unmarshal (IORequest::_ptr_type rq, size_t count, T* dst)
	{
		if (is_CDR) {
			void* pbuf = nullptr;
			if (rq->unmarshal_CDR (alignof (T), sizeof (T) * count, pbuf)) {
				for (T* src = (T*)pbuf, *end = src + count; src != end; ++src, ++dst) {
					Type <T>::byteswap (*src);
					*dst = *src;
				}
			} else
				real_copy ((const T*)pbuf, (const T*)pbuf + count, dst);
		} else
			rq->unmarshal_fixed_len (alignof (T), sizeof (T), count, dst, Type <T>::type_code ());
	}

	static void marshal_CDR (const T* src, size_t count, IORequest::_ptr_type rq)
	{
		if (is_CDR)
			marshal_CDR_stub ();
		else {
			CDR* dst = (CDR*)rq->marshal_get_buffer (alignof (CDR), sizeof (CDR) * count);
			for (const T* src = (const T*)pbuf, *end = src + count; src != end; ++src, ++dst) {
				::new (dst) CDR (*src);
			}
		}
	}

	static void unmarshal_CDR (IORequest::_ptr_type rq, size_t count, T* dst)
	{
		if (is_CDR)
			marshal_CDR_stub ();
		else {
			void* pbuf = nullptr;
			if (rq->unmarshal_CDR (alignof (CDR), sizeof (CDR) * count, pbuf)) {
				for (CDR* src = (CDR*)pbuf, *end = src + count; src != end; ++src, ++dst) {
					Type <T>::byteswap (*src);
					*dst = *src;
				}
			} else {
				for (CDR* src = (CDR*)pbuf, *end = src + count; src != end; ++src, ++dst) {
					*dst = *src;
				}
			}
		}
	}

};

}
}

#endif
