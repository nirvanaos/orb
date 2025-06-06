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
#include <Nirvana/time_defs.h>
#include "exceptions.h"
#include "Type.h"
#include "UnknownUserException.h"
#include "ServantBase.h"
#include "LocalObject.h"
#include "ValueBaseImpl.h"
#include "ValueBasePOA.h"
#include "ValueStatic.h"
#include "AbstractBaseImpl.h"
#include "AbstractBasePOA.h"
#include "PseudoBase.h"
#include "ValueFactoryBase.h"
#include "Union.h"
#include "TraitsServantPseudo.h"
#include "TypeCodeValue.h"
#include "ExceptionHolder.h"
#include "TypeCodeTypeDef.h"
#include "TypeCodeString.h"
#include "TypeCodeArray.h"
#include "TypeCodeSequence.h"
#include "TypeCodeFixed.h"
#include "String_compat.h"
#include "FindInterface.h"
#include "DefaultValueRefCountBase.h"
#include "ServantLocator_native.h"
#include "ValueFactoryImpl.h"
#include "TypeCodeValueAbstract.h"
#include "TypeCodeInterface.h"
#include "TypeCodeExceptionEmpty.h"
#include "SoftFloat_impl.h"
#include "DomainManager.h"
#include "Messaging.h"
#include "ValueCreator.h"
#include "2base.h"
#include "SystemException_impl.h"

namespace Nirvana {

/// Vendor Minor Codeset ID.
const uint32_t VMCID = 0xFFFFF;

inline uint32_t make_minor_errno (int_fast16_t err) noexcept
{
	assert (err <= 0xFFF);
	return (VMCID << 12) | err;
}

inline int_fast16_t get_minor_errno (uint32_t minor) noexcept
{
	if ((minor & 0xFFFFF000)  == (VMCID << 12))
		return minor & 0xFFF;
	else
		return 0;
}

}

namespace CORBA {

inline
ORB::_ptr_type ORB_init (int&, char**, Internal::String_in) noexcept
{
	return the_orb;
}

}

#endif
