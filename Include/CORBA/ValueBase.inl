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
#ifndef NIRVANA_ORB_VALUEBASE_INL_
#define NIRVANA_ORB_VALUEBASE_INL_
#pragma once

#include "ValueBase.h"
#include "IORequestClient.h"

namespace CORBA {
namespace Internal {

template <class Itf> inline
void TypeValue <Itf>::marshal_in (I_ptr <Itf> src, IORequest_ptr rq)
{
	rq->marshal_value (src);
}

template <class Itf> inline
void TypeValue <Itf>::unmarshal (IORequest_ptr rq, I_ref <Itf>& dst)
{
	// Itf may be not completely defined so we use reinterpret_cast
	rq->unmarshal_value (RepIdOf <Itf>::id, reinterpret_cast <I_ref <Interface>&> (dst));
}

}
}

#endif
