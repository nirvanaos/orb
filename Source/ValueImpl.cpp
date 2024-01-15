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

namespace CORBA {
namespace Internal {

Interface* ValueNonTruncatable::__truncatable_base (Bridge <ValueBase>*, Interface*)
{
	return nullptr;
}

Interface* ValueAbstract::__copy_value (Bridge <ValueBase>*, Interface*)
{
	return nullptr;
}

Interface* ValueAbstract::__factory (Bridge <ValueBase>*, Interface*)
{
	return nullptr;
}

void ValueAbstract::__marshal (Bridge <ValueBase>*, Interface*, Interface* _env)
{
	set_MARSHAL (_env, MAKE_OMG_MINOR (1)); // Unable to locate value factory.
}

void ValueAbstract::__unmarshal (Bridge <ValueBase>*, Interface*, Interface* _env)
{
	set_MARSHAL (_env, MAKE_OMG_MINOR (1)); // Unable to locate value factory.
}

}
}
