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
#ifndef NIRVANA_ORB_NAMEDVALUE_H_
#define NIRVANA_ORB_NAMEDVALUE_H_

#include "Any.h"

namespace CORBA {

class NamedValue;
typedef Nirvana::I_ptr <NamedValue> NamedValue_ptr;
typedef Nirvana::I_var <NamedValue> NamedValue_var;
typedef Nirvana::I_out <NamedValue> NamedValue_out;
typedef Nirvana::I_inout <NamedValue> NamedValue_inout;

namespace Nirvana {

BRIDGE_BEGIN (NamedValue, CORBA_REPOSITORY_ID ("NamedValue"))
ABI_VT_ret <String> (*name) (Bridge <NamedValue>*, Interface*);
ABI_VT_ret <Any> (*value) (Bridge <NamedValue>*, Interface*);
Flags (*flags) (Bridge <NamedValue>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, NamedValue> :
	public T
{
public:
	const String& name ()
	{
		Environment _env;
		Bridge <Object>& _b (T::_get_bridge (_env));
		T_VT_ret <String> _ret = (_b._epv ().epv.name) (&_b, &_env);
		_env.check ();
		return _ret;
	}

	const Any& value ()
	{
		Environment _env;
		Bridge <Object>& _b (T::_get_bridge (_env));
		T_VT_ret <Any> _ret = (_b._epv ().epv.value) (&_b, &_env);
		_env.check ();
		return _ret;
	}

	Flags flags ()
	{
		Environment _env;
		Bridge <Object>& _b (T::_get_bridge (_env));
		Flags _ret = (_b._epv ().epv.flags) (&_b, &_env);
		_env.check ();
		return _ret;
	}
};

}
}

#endif
