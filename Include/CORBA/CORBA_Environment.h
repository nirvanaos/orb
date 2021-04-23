/// \file CORBA_Environment.h

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
#ifndef NIRVANA_ORB_CORBA_ENVIRONMENT_H_
#define NIRVANA_ORB_CORBA_ENVIRONMENT_H_

#include "EnvironmentImpl.h"
#include "LocalImpl.h"

namespace CORBA {

//! CORBA::Environment
class Environment :
	public Nirvana::EnvironmentImpl <Environment>,
	public Nirvana::LocalImpl <Environment>
{
public:
	using Nirvana::LocalImpl <Environment>::_ptr_type;
	using Nirvana::LocalImpl <Environment>::_var_type;
	using Nirvana::LocalImpl <Environment>::_out_type;

	Environment () {}

	Environment (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment (Nirvana::EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment& operator = (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}

	Environment& operator = (Nirvana::EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}
};

namespace Nirvana {

template <>
struct Type < ::CORBA::Environment> : TypeLocal < ::CORBA::Environment>
{};

}
#ifdef LEGACY_CORBA_CPP
typedef Environment* Environment_ptr;
typedef Environment::_var_type Environment_var;
#endif

}

#endif

