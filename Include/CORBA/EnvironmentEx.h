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
#ifndef NIRVANA_ORB_ENVIRONMENTEX_H_
#define NIRVANA_ORB_ENVIRONMENTEX_H_
#pragma once

#include "EnvironmentImpl.h"

namespace CORBA {
namespace Internal {

template <class ... Exceptions>
class EnvironmentEx :
	public EnvironmentImpl <EnvironmentEx <Exceptions...> >
{
public:
	EnvironmentEx () {}

	void exception_set (Short code, const char* rep_id, void* param)
	{
		EnvironmentBase::exception_set (code, rep_id, param, user_exceptions_);
	}

private:
	static const ExceptionEntry user_exceptions_[];
};

template <class ... Exceptions>
const ExceptionEntry EnvironmentEx <Exceptions...>::user_exceptions_[] = {
	{ RepIdOf <Exceptions>::id, sizeof (Exceptions), ::CORBA::Internal::construct <Exceptions> }...,
	{0}
};

}
}

#endif
