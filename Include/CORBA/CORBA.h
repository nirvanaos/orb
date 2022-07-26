/// \file CORBA.h
/// The main header.

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
#ifndef NIRVANA_ORB_CORBA_H_
#define NIRVANA_ORB_CORBA_H_
#pragma once

/// Define macro LEGACY_CORBA_CPP to support of the legacy C++ mapping specification 1.3.
//#define LEGACY_CORBA_CPP

#include <Nirvana/NirvanaBase.h>
#include "exceptions.h"
#include "Type.h"
#include "String_compat.h"
#include "UnknownUserException.h"
#include "CORBA_Environment.h"
#include "core_objects.h"
#include "LocalObject.h"
#include "ValueBaseImpl.h"
#include "AbstractBaseImpl.h"
#include "AbstractBasePOA.h"
#include "PseudoBase.h"
#include "ValueFactoryBase.h"
#include "Union.h"
#include "TypeCodeTypeDef.h"
#include "ORB.h"
#include "InterfaceRepository.h"
#include "ServantLocator_native.h"

namespace CORBA {

inline
ORB::_ptr_type ORB_init (int&, char**, Internal::String_in) NIRVANA_NOEXCEPT
{
	return g_ORB;
}

}

#endif
