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
#ifndef NIRVANA_ORB_RQPROCWRAPPER_H_
#define NIRVANA_ORB_RQPROCWRAPPER_H_

#include "../I_ptr.h"

namespace CORBA {
namespace Internal {

class Interface;
class IORequest;

typedef void (*RqProcInternal) (Interface* servant, I_ptr <IORequest> call);

bool call_request_proc (RqProcInternal proc, Interface* servant, Interface* call) noexcept;

// Request procedure wrapper
template <class Param>
class RqProcWrapper
{
public:
	template <void (*proc) (Param*, I_ptr <IORequest>)>
	static bool call (Interface* servant, Interface* call) noexcept
	{
		return call_request_proc ((RqProcInternal)proc, servant, call);
	}
};

}
}

#endif
