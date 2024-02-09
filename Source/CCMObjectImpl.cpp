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
#include "../../pch/pch.h"
#include <CORBA/CCMObjectImpl.h>
#include <CORBA/ccm/CCM_Cookie_s.h>

namespace CORBA {
namespace Internal {

class CCMCookie : public IDL::traits <Components::Cookie>::Servant <CCMCookie>
{
	typedef IDL::traits <Components::Cookie>::Servant <CCMCookie> Base;

public:
	CCMCookie (size_t i) :
		Base (CORBA::OctetSeq ((const Octet*)&i, (const Octet*)(&i + 1)))
	{}

	size_t index () const
	{
		if (cookieValue ().size () != sizeof (size_t))
			throw Components::InvalidConnection ();
		return *(size_t*)cookieValue ().data ();
	}
};

Components::Cookie::_ref_type index2cookie (size_t i)
{
	return make_reference <CCMCookie> (i);
}

size_t cookie2index (Components::Cookie::_ptr_type cookie)
{
	if (!cookie)
		throw Components::CookieRequired ();
	return static_cast <const CCMCookie*> (static_cast <Bridge <Components::Cookie>*> (&cookie))->index ();
}

}
}
