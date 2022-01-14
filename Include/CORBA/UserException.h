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
#ifndef NIRVANA_ORB_USEREXCEPTION_H_
#define NIRVANA_ORB_USEREXCEPTION_H_
#pragma once

#include "Exception.h"

namespace CORBA {

class UserException : public Exception
{
	virtual Code __code () const NIRVANA_NOEXCEPT
	{
		return EC_USER_EXCEPTION;
	}

public:
	static const UserException* _downcast (const Exception* ep) NIRVANA_NOEXCEPT
	{
		return (ep && (ep->__code () < 0)) ? static_cast <const UserException*> (ep) : 0;
	}

	static UserException* _downcast (Exception* ep) NIRVANA_NOEXCEPT
	{
		return const_cast <UserException*> (_downcast ((const Exception*)ep));
	}

	static const UserException* _narrow (const Exception* ep) NIRVANA_NOEXCEPT
	{
		return _downcast (ep);
	}

	static UserException* _narrow (Exception* ep) NIRVANA_NOEXCEPT
	{
		return _downcast (ep);
	}

protected:
	UserException ()
	{}
};

}

#define NIRVANA_EXCEPTION_DEF(Parent, E) const Parent E* Parent E::_downcast (const ::CORBA::Exception* ep) NIRVANA_NOEXCEPT {\
return (ep && ::CORBA::Internal::RepositoryId::compatible (ep->_rep_id (), ::CORBA::Internal::RepIdOf <Parent E>::repository_id_))\
? &static_cast <const Parent E&> (*ep) : nullptr; }\
const char* Parent E::_rep_id () const NIRVANA_NOEXCEPT { return ::CORBA::Internal::RepIdOf <Parent E>::repository_id_; }\
GNU_OPTNONE ::CORBA::Internal::I_ptr <::CORBA::TypeCode> Parent E::__type_code () const NIRVANA_NOEXCEPT { return Parent _tc_##E; }

#endif
