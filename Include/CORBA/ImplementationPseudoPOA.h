/// \file
/// \brief Pseudo object servant virtual implementation.
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
#ifndef NIRVANA_ORB_IMPLEMENTATIONPSEUDOPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPSEUDOPOA_H_
#pragma once

#include "ServantImpl.h"
#include "I_ptr.h"
#include "ReferenceCounterPOA.h"

namespace CORBA {
namespace Internal {

//! \class ImplementationPseudoPOA
//!
//! \brief POA-style (virtual) implementation of the pseudo interface.
//!
//! \tparam Primary Primary interface.

template <class Primary>
class NIRVANA_NOVTABLE ImplementationPseudoPOA :
	public virtual ServantPOA <ReferenceCounter>,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	typedef Primary PrimaryInterface;

	//! \fn I_ptr <Primary> _get_ptr ()
	//!
	//! \brief Gets the pointer.
	//!   Works like _this() method but doesn't increment the reference counter.
	//!
	//! \return The primary interface pointer.

	I_ptr <Primary> _get_ptr () NIRVANA_NOEXCEPT
	{
		return I_ptr <Primary> (&static_cast <Primary&> (static_cast <Bridge <Primary>&> (*this)));
	}

protected:
	ImplementationPseudoPOA () NIRVANA_NOEXCEPT
	{}
};

}
}

#endif
