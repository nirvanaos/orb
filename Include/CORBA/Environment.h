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
#ifndef NIRVANA_ORB_ENVIRONMENT_H_
#define NIRVANA_ORB_ENVIRONMENT_H_

#include "EnvironmentImpl.h"

namespace CORBA {
namespace Nirvana {

class Environment :
	public EnvironmentImpl <Environment>
{
public:
	Environment () {}
	
	Environment (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment (EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
	}

	Environment& operator = (Environment&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}

	Environment& operator = (EnvironmentBase&& src) NIRVANA_NOEXCEPT
	{
		move_from (src);
		return *this;
	}
};

}
}

#endif
