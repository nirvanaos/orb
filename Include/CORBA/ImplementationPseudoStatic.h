/// \file
/// \brief Pseudo object static servant implementation.
/// 
/// This header is intended for core and proxy code.

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
#ifndef NIRVANA_ORB_IMPLEMENTATIONPSEUDOSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONPSEUDOSTATIC_H_
#pragma once

#include "ServantStatic.h"
#include "LifeCycleStatic.h"

namespace CORBA {
namespace Internal {

//! \class ImplementationPseudoStatic
//!
//! \brief Static implementation of a pseudo interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.

template <class S, class Primary>
class ImplementationPseudoStatic :
	public InterfaceStaticBase <S, Primary>,
	public ServantTraitsStatic <S>,
	public LifeCycleStatic
{
public:
	typedef Primary PrimaryInterface;

	//! \brief Gets the pointer.
	//!   Works like _this() method but doesn't increment the reference counter.
	//!
	//! \return The primary interface pointer.
	static Primary* _get_ptr () noexcept
	{
		return static_cast <Primary*> (InterfaceStaticBase <S, Primary>::_bridge ());
	}
};

}
}

#endif
