/// \file
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
#ifndef NIRVANA_ORB_EXCEPTIONHOLDER_H_
#define NIRVANA_ORB_EXCEPTIONHOLDER_H_
#pragma once

#include "AMI.h"
#include "ExceptionSet.h"
#include "ExceptionList.h"

namespace CORBA {
namespace Internal {

template <class ... Exceptions>
inline void set_user_exceptions (::Messaging::ExceptionHolder::_ptr_type eh)
{
	eh->set_user_exceptions (ExceptionSet <Exceptions...>::entries (), sizeof... (Exceptions));
}

void raise_holder_exception (Bridge < ::Messaging::ExceptionHolder>& _b, Environment& _env,
	const ::Dynamic::ExceptionList* exc_list = nullptr);

template <class T>
void Client <T, ::Messaging::ExceptionHolder>::raise_exception ()
{
	Environment _env;
	Bridge < ::Messaging::ExceptionHolder>& _b (T::_get_bridge (_env));
	raise_holder_exception (_b, _env);
}

template <class T>
void Client <T, ::Messaging::ExceptionHolder>::raise_exception_with_list (
	Type < ::Dynamic::ExceptionList>::C_in exc_list)
{
	Environment _env;
	Bridge < ::Messaging::ExceptionHolder>& _b (T::_get_bridge (_env));
	raise_holder_exception (_b, _env, &Type < ::Dynamic::ExceptionList>::in (&exc_list));
}

}
}

#endif
