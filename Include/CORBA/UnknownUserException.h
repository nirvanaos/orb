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
#ifndef NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_
#define NIRVANA_ORB_UNKNOWNUSEREXCEPTION_H_
#pragma once

#include "UserException.h"
#include "Any.h"

namespace CORBA {

extern NIRVANA_STATIC_IMPORT Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_UnknownUserException;

class UnknownUserException : public UserException
{
	virtual Code __code () const noexcept
	{
		return EC_UNKNOWN_USER_EXCEPTION;
	}

public:
	NIRVANA_EXCEPTION_DCL (UnknownUserException);

	typedef Any _Data;

	Any& exception ()
	{
		return exception_;
	}

	UnknownUserException ()
	{}

	explicit UnknownUserException (Any&& exc) :
		exception_ (std::move (exc))
	{}

	~UnknownUserException ()
	{}

private:
	virtual void* __data () noexcept
	{
		return &exception_;
	}

private:
	Any exception_;
};

namespace Internal {

template <>
const Char RepIdOf <UnknownUserException>::id [] = CORBA_REPOSITORY_ID ("UnknownUserException");
}

}

namespace Messaging {

typedef CORBA::UnknownUserException UserExceptionBase;

}

#endif
