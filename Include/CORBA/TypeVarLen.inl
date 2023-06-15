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

template <typename First, typename Last> inline
void marshal_members (const First* first, const Last* last, IORequest_ptr rq)
{
	rq->marshal (alignof (First), (const Octet*)(last + 1) - (const Octet*)first, first);
}

template <typename First, typename Last> inline
bool unmarshal_members (IORequest_ptr rq, First* first, const Last* last)
{
	return rq->unmarshal (alignof (First), (const Octet*)(last + 1) - (const Octet*)first, first);
}

}
}

#endif
