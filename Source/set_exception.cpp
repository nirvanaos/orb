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
#include "../../pch/pch.h"
#include <CORBA/set_exception.h>
#include <CORBA/exceptions.h>
#include <CORBA/Environment_c.h>
#include <CORBA/system_exceptions.h>

#define DEFINE_FUNCTION(e) void set_##e (Interface* env) noexcept { set_exception (env, SystemException::EC_##e, RepIdOf<e>::id, nullptr);}

namespace CORBA {
namespace Internal {

SYSTEM_EXCEPTIONS (DEFINE_FUNCTION)

}
}
