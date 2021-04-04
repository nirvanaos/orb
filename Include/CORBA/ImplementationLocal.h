/// \file
/// \author Igor Popov
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

#include "LocalObjectImpl.h"
#include "FindInterface.h"

namespace CORBA {
namespace Nirvana {

//! \class ImplementationLocal
//!
//! \brief An implementation of a local interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.
//!    Don't include AbstractBase in base list.

template <class S, class Primary, class ... Bases>
class ImplementationLocal :
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>,
	public InterfaceImpl <S, LocalObject> // LocalObject implementation must be constructed after all interfaces.
{
public:
	Interface_ptr _query_interface (const String& id)
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}

	I_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (LocalObjectLink::_get_proxy ());
	}

protected:
	ImplementationLocal ()
	{}
};

}
}

#endif
