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
#ifndef NIRVANA_ORB_STRING_INL_
#define NIRVANA_ORB_STRING_INL_
#pragma once

#include "String.h"
#include "IORequestClient.h"

namespace CORBA {
namespace Internal {

inline
void Type <StringT <Char> >::marshal_in (const Var& src, IORequest_ptr rq)
{
	rq->marshal_string (const_cast <Var&> (src), false);
}

inline
void Type <StringT <Char> >::marshal_out (Var& src, IORequest_ptr rq)
{
	rq->marshal_string (src, true);
}

inline
void Type <StringT <Char> >::unmarshal (IORequest_ptr rq, Var& dst)
{
	rq->unmarshal_string (dst);
}

inline
I_ptr <TypeCode> Type <StringT <Char> >::type_code () NIRVANA_NOEXCEPT
{
	return _tc_string;
}

inline
void Type <StringT <WChar> >::marshal_in (const Var& src, IORequest_ptr rq)
{
	rq->marshal_wstring (const_cast <Var&> (src), false);
}

inline
void Type <StringT <WChar> >::marshal_out (Var& src, IORequest_ptr rq)
{
	rq->marshal_wstring (src, true);
}

inline
void Type <StringT <WChar> >::unmarshal (IORequest_ptr rq, Var& dst)
{
	rq->unmarshal_wstring (dst);
}

inline
I_ptr <TypeCode> Type <StringT <WChar> >::type_code () NIRVANA_NOEXCEPT
{
	return _tc_wstring;
}

}
}

#endif
