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
#ifndef NIRVANA_ORB_TYPEVARLENA_H_
#define NIRVANA_ORB_TYPEVARLENA_H_
#pragma once

#include "Type_forward.h"

namespace CORBA {
namespace Internal {

/// Variable-legth type array marshaling/unmarshaling.
template <class T, class Var>
struct TypeVarLenA
{
	static void marshal_in_a (const Var* src, size_t count, IORequest_ptr rq);
	static void marshal_out_a (Var* src, size_t count, IORequest_ptr rq);
	static void unmarshal_a (IORequest_ptr rq, size_t count, Var* dst);
};

template <class T, class Var>
void TypeVarLenA <T, Var>::marshal_in_a (const Var* src, size_t count, IORequest_ptr rq)
{
	for (const Var* end = src + count; src != end; ++src) {
		Type <T>::marshal_in (*src, rq);
	}
}

template <class T, class Var>
void TypeVarLenA <T, Var>::marshal_out_a (Var* src, size_t count, IORequest_ptr rq)
{
	for (Var* end = src + count; src != end; ++src) {
		Type <T>::marshal_out (*src, rq);
	}
}

template <class T, class Var>
void TypeVarLenA <T, Var>::unmarshal_a (IORequest_ptr rq, size_t count, Var* dst)
{
	for (Var* end = dst + count; dst != end; ++dst) {
		Type <T>::unmarshal (rq, *dst);
	}
}

}
}

#endif
