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
#ifndef NIRVANA_ORB_IMPLEMENTATIONLOCALSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONLOCALSTATIC_H_
#pragma once

#include "LocalObjectStatic.h"

namespace CORBA {
namespace Internal {

//! \brief Static implementation of a local interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationLocalStatic :
	public InterfaceStatic <S, LocalObject>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{
public:
	typedef Primary PrimaryInterface;

	static I_ptr <Interface> _query_interface (const String& id)
	{
		return FindInterface <Primary, Bases...>::find (*(S*)0, id);
	}

	static typename Type <Primary>::VRet _this ()
	{
		return InterfaceStatic <S, LocalObject>::_get_proxy ().template downcast <Primary> ();
	}
};

}
}

#endif
