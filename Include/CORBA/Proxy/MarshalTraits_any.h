/// \file MarshalTraits_any.h
/// \brief Declares the MarshalTraits <Any>.
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
#ifndef NIRVANA_ORB_MARSHALTRAITS_ANY_H_
#define NIRVANA_ORB_MARSHALTRAITS_ANY_H_

#include "MarshalTraits_forward.h"
#include "../Any.h"
#include "Marshal.h"
#include "Unmarshal.h"

namespace CORBA {
namespace Nirvana {

template <>
struct MarshalTraits <Any>
{
	static const bool has_marshal = true;

	typedef ABI <Any> ABI;

	static void marshal_in (const Any& src, Marshal_ptr marshaler, ABI& dst);
	static void marshal_out (Any& src, Marshal_ptr marshaler, ABI& dst);
	static void unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, Any& dst);
};

}
}

#endif
