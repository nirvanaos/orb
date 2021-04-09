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
#ifndef NIRVANA_ORB_TYPE_H_
#define NIRVANA_ORB_TYPE_H_

#include "TypeVarLen.h"
#include "TypeEnum.h"
#include "Type_interface.h"
#include "TypeCode.h"

namespace CORBA {
namespace Nirvana {

// Marshaling helpers

template <class T, class ABI> inline
void _marshal_in (const T& src, Marshal_ptr marshaler, ABI& dst)
{
	Type <T>::marshal_in (src, marshaler, dst);
}

template <class I> inline
void _marshal_in (const I_ptr <I> src, Marshal_ptr marshaler, Interface*& dst)
{
	Type <I_var <I> >::marshal_in (src, marshaler, dst);
}

inline
void _marshal_in (const TypeCode_ptr src, Marshal_ptr marshaler, Interface*& dst)
{
	Type <TypeCode_var>::marshal_in (src, marshaler, dst);
}

template <class T, class ABI> inline
void _marshal_out (T& src, Marshal_ptr marshaler, ABI& dst)
{
	Type <T>::marshal_out (src, marshaler, dst);
}

template <class T, class ABI> inline
void _unmarshal (const ABI& src, Unmarshal_ptr unmarshaler, T& dst)
{
	Type <T>::unmarshal (src, unmarshaler, dst);
}

}
}

// Compatibility with modern C11 mapping
namespace IDL {

template <class T>
struct traits
{
	typedef typename CORBA::Nirvana::Type <T>::C_var value_type;
	typedef typename CORBA::Nirvana::Type <T>::C_in in_type;
	typedef typename CORBA::Nirvana::Type <T>::C_out out_type;
	typedef typename CORBA::Nirvana::Type <T>::C_inout inout_type;
};

}

#endif
