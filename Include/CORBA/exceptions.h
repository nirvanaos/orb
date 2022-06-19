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
#ifndef NIRVANA_ORB_EXCEPTIONS_H_
#define NIRVANA_ORB_EXCEPTIONS_H_
#pragma once

#include "SystemException.h"
#include <Nirvana/ImportInterface.h>

#define DECLARE_SYSTEM_EXCEPTION(E) \
extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_##E;\
class E : public SystemException {\
NIRVANA_EXCEPTION_DCL(E)\
E () {}\
E (ULong minor, CompletionStatus status = COMPLETED_NO) : SystemException (minor, status) {}\
virtual Code __code () const NIRVANA_NOEXCEPT { return EC_##E; }};

#define DECLARE_CORBA_EXCEPTION(E) DECLARE_SYSTEM_EXCEPTION(E)\
namespace Internal { template <> const Char RepIdOf <E>::id [] = "IDL:omg.org/CORBA/" #E ":1.0"; }

#define DECLARE_NIRVANA_EXCEPTION(E) DECLARE_SYSTEM_EXCEPTION(E)\
namespace Internal { template <> const Char RepIdOf <E>::id [] = "IDL:CORBA/" #E ":1.0"; }

namespace CORBA {

CORBA_EXCEPTIONS (DECLARE_CORBA_EXCEPTION)
NIRVANA_EXCEPTIONS (DECLARE_NIRVANA_EXCEPTION)

}

#undef DECLARE_SYSTEM_EXCEPTION
#undef DECLARE_CORBA_EXCEPTION
#undef DECLARE_NIRVANA_EXCEPTION

#endif
