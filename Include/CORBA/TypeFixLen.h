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
#include "IORequest.h"
#include <type_traits>

namespace CORBA {
namespace Internal {

/// Fixed-length data types.
/// Passed by value if sizeof (T) <= 2 * sizeof (size_t).
/// 
/// \tparam T The variable type.
/// \tparam TABI The ABI type.
template <typename T, typename TABI = T>
struct TypeFixLen : std::conditional_t <sizeof (T) <= 2 * sizeof (size_t), TypeByVal <T, TABI>, TypeByRef <T, TABI> >
{
	static void marshal_in (const T* src, size_t count, IORequest::_ptr_type rq)
	{
		rq->marshal (alignof (T), sizeof (T) * count, src);
	}

	static void marshal_out (T* src, size_t count, IORequest::_ptr_type rq)
	{
		marshal_in (src, count, rq);
	}

	static void unmarshal (IORequest::_ptr_type rq, size_t count, T* dst)
	{
		void* pbuf = nullptr;
		if (rq->unmarshal (alignof (T), sizeof (T) * count, pbuf)) {
			for (T* src = (T*)pbuf, *end = src + count; src != end; ++src, ++dst) {
				Type <T>::byteswap (*src);
				*dst = *src;
			}
		} else
			real_copy ((const T*)pbuf, (const T*)pbuf + count, dst);
	}

};

}
}

#endif
