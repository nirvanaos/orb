/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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

#include "UserException.h"
#include "Any.h"

namespace CORBA {

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_UnknownUserException;

class UnknownUserException : public UserException
{
public:
	DECLARE_EXCEPTION (UnknownUserException);

	typedef Any _Data;

	const Any& exception () const
	{
		return _data;
	}

	UnknownUserException (Any&& exc) :
		_data (std::move (exc))
	{}

	virtual void* __data () NIRVANA_NOEXCEPT;

private:
	_Data _data;
};

}

#endif
