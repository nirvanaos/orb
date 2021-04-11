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
#ifndef NIRVANA_ORB_LIFECYCLESTATIC_H_
#define NIRVANA_ORB_LIFECYCLESTATIC_H_

#include "Client.h"
#include <Nirvana/Module.h>
#include <Nirvana/core_objects.h>

namespace CORBA {
namespace Nirvana {

class LifeCycleStatic
{
public:
	template <class I>
	static Interface* __duplicate (Interface* itf, Interface*)
	{
		::Nirvana::Module::_duplicate (::Nirvana::g_module);
		return itf;
	}

	template <class I>
	static void __release (Interface*)
	{
		release ((::Nirvana::Module_ptr)::Nirvana::g_module);
	}
};

}
}

#endif
