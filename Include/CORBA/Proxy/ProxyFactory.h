/// \file ProxyFactory.h
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
#ifndef NIRVANA_ORB_PROXYFACTORY_H_
#define NIRVANA_ORB_PROXYFACTORY_H_

#include "IOReference.h"
#include "../DynamicServant.h"

namespace CORBA {
namespace Nirvana {

class IOReference;
typedef I_ptr <IOReference> IOReference_ptr;
typedef I_var <IOReference> IOReference_var;

class ProxyFactory;
typedef I_ptr <ProxyFactory> ProxyFactory_ptr;
typedef I_var <ProxyFactory> ProxyFactory_var;

/*
~~~{.idl}
pseudo interface ProxyFactory {
	const InterfaceMetadata* metadata ();
	Interface create_proxy (IOReference target, short interface_idx, out DynamicServant deleter);
};
~~~
*/

NIRVANA_BRIDGE_BEGIN (ProxyFactory, CORBA_NIRVANA_REPOSITORY_ID ("ProxyFactory"))
const InterfaceMetadata* metadata;
Interface* (*create_proxy) (Bridge <ProxyFactory>*, 
	Interface*, UShort interface_idx,
	Interface**, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ProxyFactory> :
	public T
{
public:
	const InterfaceMetadata* metadata ()
	{
		Environment _env;
		return T::_get_bridge (_env)._epv ().epv.metadata;
	}

	Interface_ptr create_proxy (
		I_in <IOReference> target, UShort interface_idx, 
		I_out <DynamicServant> deleter);
};

template <class T>
Interface_ptr Client <T, ProxyFactory>::create_proxy (
	I_in <IOReference> target, UShort interface_idx,
	I_out <DynamicServant> deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	I_VT_ret <Interface> _ret = (_b._epv ().epv.create_proxy) (&_b, &target, interface_idx, &deleter, &_env);
	_env.check ();
	return _ret;
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
