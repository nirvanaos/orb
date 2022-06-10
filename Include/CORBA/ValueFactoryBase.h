/// \file
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
#ifndef NIRVANA_ORB_VALUEFACTORYBASE_H_
#define NIRVANA_ORB_VALUEFACTORYBASE_H_
#pragma once

#include "ValueBase.h"

namespace CORBA {

class ValueFactoryBase;
#ifdef LEGACY_CORBA_CPP
typedef Internal::I_ptr <ValueFactoryBase> ValueFactory;
typedef Internal::I_var <ValueBase> ValueBase_var;
typedef ValueBase_var& ValueBase_out;
#endif

namespace Internal {

template <>
struct Type <ValueFactoryBase> : TypeItf <ValueFactoryBase>
{};

NIRVANA_BRIDGE_BEGIN (ValueFactoryBase, CORBA_REPOSITORY_ID ("ValueFactoryBase"))
Interface* (*create_for_unmarshal) (Bridge <ValueFactoryBase>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ValueFactoryBase> :
	public T
{
private:
	Type <ValueBase>::Var create_for_unmarshal ();
};

template <class T>
Type <ValueBase>::Var Client <T, ValueFactoryBase>::create_for_unmarshal ()
{
	Environment _env;
	Bridge <ValueFactoryBase>& _b (T::_get_bridge (_env));
	Type <ValueBase>::C_ret _ret = (_b._epv ().epv.create_for_unmarshal) (&_b, &_env);
	_env.check ();
	return _ret;
}

}

class ValueFactoryBase :
	public Internal::ClientInterfacePrimary <ValueFactoryBase>
{};

namespace Internal {

template <class Primary>
class ClientInterfaceBase <Primary, ValueFactoryBase> :
	public Client <ClientBase <Primary, ValueFactoryBase>, ValueFactoryBase>
{
};

}
}

#endif

