/// \file ImplementationPseudo.h
/// \brief Pseudo object servant implementation.
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
#ifndef NIRVANA_ORB_IMPLEMENTATIONPSEUDO_H_
#define NIRVANA_ORB_IMPLEMENTATIONPSEUDO_H_
#pragma once

#include "ServantImpl.h"
#include "I_ptr.h"

namespace CORBA {
namespace Internal {

//! \class ImplementationPseudo
//!
//! \brief An implementation of a pseudo object (like proxy).
//!
//! You also have to derive your servant from some life cycle implementation,
//! usually `LifeCycleServant`.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.

template <class S, class Primary>
class ImplementationPseudo :
	public InterfaceImplBase <S, Primary>,
	public ServantTraits <S>
{
public:
	typedef Primary PrimaryInterface;

	I_ptr <Primary> _get_ptr () noexcept
	{
		return I_ptr <Primary> (&static_cast <Primary&> (static_cast <Bridge <Primary>&> (*this)));
	}

protected:
	ImplementationPseudo () noexcept
	{}
};

}
}

#endif
