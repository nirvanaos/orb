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
#ifndef NIRVANA_ORB_EXCEPTIONLIST_H_
#define NIRVANA_ORB_EXCEPTIONLIST_H_
#pragma once

#include "Exception.h"
#include "Sequence.h"
#include "TypeFixLen.h"

namespace Dynamic {

class ExceptionList : public IDL::Sequence <CORBA::Internal::ExceptionEntry>
{
public:
	template <class Ex>
	void add ()
	{
		push_back ({ CORBA::Internal::RepIdOf <Ex>::id, sizeof (Ex), ::CORBA::Internal::construct <Ex> });
	}
};

}

namespace CORBA {
namespace Internal {

template <>
struct Type <ExceptionEntry> : TypeFixLen <ExceptionEntry, false>
{};

template <>
struct Type <Dynamic::ExceptionList> : Type <Sequence <ExceptionEntry> >
{};

typedef const ExceptionEntry* ExceptionEntryPtr;

}
}

#endif
