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
#pragma once

#include "EnvironmentImpl.h"
#include "Serverless.h"

namespace CORBA {

//! CORBA::Environment
class Environment :
	public Internal::EnvironmentImpl <Environment>,
	public Internal::Serverless <Environment>
{
public:
	using Internal::Serverless <Environment>::_ptr_type;
	using Internal::Serverless <Environment>::_out_type;
#ifdef LEGACY_CORBA_CPP
	using Internal::Serverless <Environment>::_var_type;
#else
	using Internal::Serverless <Environment>::_ref_type;
#endif

	Environment () {}

	Environment (Environment&& src) noexcept
	{
		move_from (src);
	}

	Environment (Internal::EnvironmentBase&& src) noexcept
	{
		move_from (src);
	}

	Environment& operator = (Environment&& src) noexcept
	{
		move_from (src);
		return *this;
	}

	Environment& operator = (Internal::EnvironmentBase&& src) noexcept
	{
		move_from (src);
		return *this;
	}
};

namespace Internal {

template <>
struct Type < ::CORBA::Environment> : TypeServerless < ::CORBA::Environment>
{};

}

#ifdef LEGACY_CORBA_CPP
typedef Environment* Environment_ptr;
typedef Environment::_var_type Environment_var;
#endif

}

#endif

