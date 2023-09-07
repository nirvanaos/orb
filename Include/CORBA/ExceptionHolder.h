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

void raise_exception (IORequest::_ptr_type rq, bool is_system_exception,
	const ExceptionEntry* user_exceptions, size_t user_exceptions_cnt);

template <class T>
void Client <T, ::Messaging::ExceptionHolder>::raise_exception ()
{
	Environment _env;
	Bridge < ::Messaging::ExceptionHolder>& _b (T::_get_bridge (_env));
	Type <bool>::ABI is_system_exception;
	const ExceptionEntry* user_exceptions;
	size_t user_exceptions_cnt;
	Type <IORequest>::C_ret rq ((_b._epv ().epv.get_exception) (&_b, &is_system_exception,
		&user_exceptions, &user_exceptions_cnt, &_env));
	_env.check ();
	Internal::raise_exception (Type <IORequest>::VRet (rq), is_system_exception, user_exceptions, user_exceptions_cnt);
}

template <class T>
void Client <T, ::Messaging::ExceptionHolder>::raise_exception_with_list (
	Type < ::Dynamic::ExceptionList>::C_in exc_list)
{
	Environment _env;
	Bridge < ::Messaging::ExceptionHolder>& _b (T::_get_bridge (_env));
	const ExceptionEntry* user_exceptions;
	size_t user_exceptions_cnt;
	Type <Any>::C_ret exc ((_b._epv ().epv.get_exception) (&_b, &user_exceptions, &user_exceptions_cnt, &_env));
	_env.check ();
	const ::Dynamic::ExceptionList& excl = Type < ::Dynamic::ExceptionList>::in (&exc_list);
	raise_exception (std::move (exc), excl.data (), excl.size ());
}

}
}

#endif
