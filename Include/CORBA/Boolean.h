/// \file Boolean.h
/// Provides ABI for the bool type.
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
#ifndef NIRVANA_ORB_BOOLEAN_H_
#define NIRVANA_ORB_BOOLEAN_H_

#include <Nirvana/NirvanaBase.h>
#include "TypeFixLen.h"

namespace CORBA {
namespace Nirvana {

template <>
struct Type <Boolean> : TypeFixLen <Boolean, ABI_boolean, char>
{
	static TypeCode_ptr type_code ();
};

typedef Type <Boolean>::C_in Boolean_in;
typedef Type <Boolean>::C_out Boolean_out;
typedef Type <Boolean>::C_inout Boolean_inout;

}
}

#endif
