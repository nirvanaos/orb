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
#ifndef NIRVANA_ORB_IMPLEMENTATIONLOCAL_H_
#define NIRVANA_ORB_IMPLEMENTATIONLOCAL_H_
#pragma once

#include "LocalObjectImpl.h"
#include "FindInterface.h"

namespace CORBA {
namespace Internal {

//! \class ImplementationLocal
//!
//! \brief An implementation of a local interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationLocal :
	public InterfaceImpl <S, Primary>,
	public InterfaceImpl <S, Bases>...,
	public ValueImpl <S, LocalObject> // LocalObject implementation must be constructed after all interfaces.
{
public:
	typedef Primary PrimaryInterface;

	Interface* _query_interface (const String& id)
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}

	I_ref <Primary> _this () const
	{
		return LocalObjectLink::_get_proxy ().template downcast <Primary> ();
	}

protected:
	ImplementationLocal ()
	{}
};

}
}

#endif
