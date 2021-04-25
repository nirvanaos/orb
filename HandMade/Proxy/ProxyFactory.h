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

namespace CORBA {
namespace Nirvana {

class IOReference;

class DynamicServant;

class ProxyFactory;

/*
~~~{.idl}
module CORBA {
module Nirvana {

native InterfaceMetadataPtr;
native InterfacePtr;

pseudo interface ProxyFactory {
	readonly attribute InterfaceMetadataPtr metadata;
	InterfacePtr create_proxy (IOReference target, short interface_idx, out DynamicServant deleter);
};

};
};
~~~
*/

NIRVANA_BRIDGE_BEGIN (ProxyFactory, CORBA_NIRVANA_REPOSITORY_ID ("ProxyFactory"))
Type <InterfaceMetadataPtr>::ABI_ret (*_get_metadata) (Bridge <ProxyFactory>*, Interface*);
Type <InterfacePtr>::ABI_ret (*create_proxy) (Bridge <ProxyFactory>*, Interface*, UShort interface_idx, Type <DynamicServantPtr>::ABI_out, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ProxyFactory> :
	public T
{
public:
	Type <InterfaceMetadataPtr>::Var metadata ();

	Type <InterfacePtr>::Var create_proxy (TypeItf <IOReference>::C_in target, UShort interface_idx, Type <DynamicServantPtr>::C_out deleter);
};

template <class T>
Type <InterfaceMetadataPtr>::Var Client <T, ProxyFactory>::metadata ()
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	Type <InterfaceMetadataPtr>::C_ret _ret = (_b._epv ().epv._get_metadata) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type <InterfacePtr>::Var Client <T, ProxyFactory>::create_proxy (TypeItf <IOReference>::C_in target, UShort interface_idx, Type <DynamicServantPtr>::C_out deleter)
{
	Environment _env;
	Bridge <ProxyFactory>& _b (T::_get_bridge (_env));
	Type <InterfacePtr>::C_ret _ret = (_b._epv ().epv.create_proxy) (&_b, &target, interface_idx, &deleter, &_env);
	_env.check ();
	return _ret;
}

class ProxyFactory : public ClientInterface <ProxyFactory>
{};

}
}

#endif
