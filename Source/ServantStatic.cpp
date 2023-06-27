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
#include <CORBA/ServantBaseStatic.h>
#include <CORBA/LocalObjectStatic.h>

namespace CORBA {
namespace Internal {

void ServantStaticDummy::__add_ref (Interface*, Interface*) noexcept
{}

void ServantStaticDummy::__remove_ref (Interface*, Interface*) noexcept
{}

ULong ServantStaticDummy::__refcount_value (Interface*, Interface*) noexcept
{
	return 1;
}

void ServantStaticDummy::__delete_object (Interface*, Interface*) noexcept
{}

}
}
