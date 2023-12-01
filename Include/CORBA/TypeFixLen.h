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
#ifndef NIRVANA_ORB_TYPEFIXLEN_H_
#define NIRVANA_ORB_TYPEFIXLEN_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include "TypeByVal.h"
#include "TypeByRef.h"
#include <type_traits>

namespace CORBA {
namespace Internal {

/// Fixed-length data types.
/// Passed by value if sizeof (T) <= 2 * sizeof (size_t).
///
/// \tparam T The variable type.
/// \tparam chk `true` if the variable has check ().
/// \tparam TABI The ABI type.
template <typename T, bool chk, typename TABI = T>
struct TypeFixLen : std::conditional <sizeof (T) <= 2 * sizeof (size_t),
	typename std::conditional <chk, TypeByValCheck <T, TABI>, TypeByVal <T, TABI> >::type,
	typename std::conditional <chk, TypeByRefCheck <T, TABI>, TypeByRef <T, TABI> >::type
>::type
{
	static const bool is_var_len = false;

	static const size_t CDR_align = alignof (TABI);
	static const size_t CDR_size = sizeof (TABI);

	inline static void marshal_in (const T& src, IORequest_ptr rq);
	inline static void marshal_in_a (const T* src, size_t count, IORequest_ptr rq);

	static void marshal_out (T& src, IORequest_ptr rq)
	{
		Type <T>::marshal_in (src, rq);
	}

	static void marshal_out_a (T* src, size_t count, IORequest_ptr rq)
	{
		Type <T>::marshal_in_a (src, count, rq);
	}

	inline static void unmarshal (IORequest_ptr rq, T& dst);
	inline static void unmarshal_a (IORequest_ptr rq, size_t count, T* dst);
};

}
}

#endif
