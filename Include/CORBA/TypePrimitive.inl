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
#ifndef NIRVANA_ORB_TYPEPRIMITIVE_INL_
#define NIRVANA_ORB_TYPEPRIMITIVE_INL_
#pragma once

#include "TypePrimitive.h"
#include "IORequestClient.h"
#include <Nirvana/real_copy.h>

namespace CORBA {
namespace Internal {

inline
void TypePrimitive <Char>::marshal_in (const Char& src, IORequest_ptr rq)
{
	rq->marshal_char (1, &src);
}

inline
void TypePrimitive <Char>::marshal_in_a (const Char* src, size_t count, IORequest_ptr rq)
{
	rq->marshal_char (count, src);
}

inline
void TypePrimitive <Char>::unmarshal (IORequest_ptr rq, Char& dst)
{
	rq->unmarshal_char (1, &dst);
}

inline
void TypePrimitive <Char>::unmarshal_a (IORequest_ptr rq, size_t count, Char* dst)
{
	rq->unmarshal_char (count, dst);
}

inline
void TypePrimitive <WChar>::marshal_in (const WChar& src, IORequest_ptr rq)
{
	rq->marshal_wchar (1, &src);
}

inline
void TypePrimitive <WChar>::marshal_in_a (const WChar* src, size_t count, IORequest_ptr rq)
{
	rq->marshal_wchar (count, src);
}

inline
void TypePrimitive <WChar>::unmarshal (IORequest_ptr rq, WChar& dst)
{
	rq->unmarshal_wchar (1, &dst);
}

inline
void TypePrimitive <WChar>::unmarshal_a (IORequest_ptr rq, size_t count, WChar* dst)
{
	rq->unmarshal_wchar (count, dst);
}

inline
void TypePrimitive <Boolean>::marshal_in (const ABI& src, IORequest_ptr rq)
{
	rq->marshal (1, 1, &src);
}

inline
void TypePrimitive <Boolean>::marshal_in_a (const ABI* src, size_t count, IORequest_ptr rq)
{
	rq->marshal (1, count, src);
}

inline
void TypePrimitive <Boolean>::unmarshal (IORequest_ptr rq, ABI& dst)
{
	rq->unmarshal (1, 1, &dst);
}

inline
void TypePrimitive <Boolean>::unmarshal_a (IORequest_ptr rq, size_t count, ABI* dst)
{
	rq->unmarshal (1, count, dst);
}

}
}

#endif
