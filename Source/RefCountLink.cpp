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
#include <CORBA/RefCountLink.h>
#include <CORBA/CORBA.h>

namespace CORBA {
namespace Internal {

RefCountLink::RefCountLink (const Bridge <DynamicServant>::EPV& epv) :
	deleter_ (epv)
{
	core_object_ = g_ORB->create_ref_cnt (&deleter_);
}

RefCountLink::RefCountLink (const RefCountLink& src) :
	deleter_ (src.deleter_)
{
	core_object_ = g_ORB->create_ref_cnt (&deleter_);
}

Interface* RefCountLink::__dup (Interface* itf, Interface*) noexcept
{
	return itf;
}

void RefCountLink::__rel (Interface*) noexcept
{}

}
}
