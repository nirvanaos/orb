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

#include "Serverless.h"
#include "ExceptionList.h"
#include "ExceptionSet.h"
#include "Any.h"

namespace Messaging {

class ExceptionHolder : public CORBA::Internal::Serverless <ExceptionHolder>
{
public:
	ExceptionHolder (CORBA::Any&& exc) :
		exception_ (std::move (exc))
	{}

	virtual void raise_exception ();

	void raise_exception_with_list (const Dynamic::ExceptionList& exc_list)
	{
		raise_exception (exc_list.data (), exc_list.size ());
	}

protected:
	void raise_exception (const CORBA::Internal::ExceptionEntry* user_exceptions,
		size_t user_exceptions_cnt) const;

private:
	CORBA::Any exception_;
};

template <class ... Exceptions>
class ExceptionHolderEx : public ExceptionHolder
{
public:
	virtual void raise_exception () override
	{
		raise_exception (CORBA::Internal::ExceptionSet <Exceptions...>::exceptions_,
			CORBA::Internal::ExceptionSet <Exceptions...>::count ());
	}
};

}

#endif
