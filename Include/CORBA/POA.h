/// \file
/// PortableServer::POA interface
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
#ifndef NIRVANA_ORB_POA_H_
#define NIRVANA_ORB_POA_H_

#include "POA_c.h"

namespace CORBA {
namespace Nirvana {

template <class T>
String Client <T, PortableServer::POA>::activate_object (I_in <PortableServer::ServantBase> servant)
{
	Environment _env;
	Bridge <PortableServer::POA>& _b (T::_get_bridge (_env));
	Type <String>::C_ret _ret = (_b._epv ().epv.activate_object) (&_b, &servant, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, PortableServer::POA>::deactivate_object (String_in oid)
{
	Environment _env;
	Bridge <PortableServer::POA>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.deactivate_object) (&_b, &oid, &_env);
	_env.check ();
}

}
}

#endif
