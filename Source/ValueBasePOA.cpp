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
#include <CORBA/ValueBasePOA.h>

namespace CORBA {
namespace Internal {

void ServantPOA <ValueBase>::_marshal (I_ptr <IORequest> rq) const
{
	throw MARSHAL (MAKE_OMG_MINOR (1)); // Unable to locate value factory.
}

void ServantPOA <ValueBase>::_unmarshal (I_ptr <IORequest> rq)
{
	throw MARSHAL (MAKE_OMG_MINOR (1)); // Unable to locate value factory.
}

TypeCode::_ptr_type ServantPOA <ValueBase>::_truncatable_base () const noexcept
{
	return nullptr;
}

ValueFactoryBase::_ptr_type ServantPOA <ValueBase>::_factory () const
{
	return nullptr;
}

}
}
