/// \file ProxyFactory_s.h
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
#ifndef NIRVANA_ORB_PROXYFACTORY_S_H_
#define NIRVANA_ORB_PROXYFACTORY_S_H_

#include "ProxyFactory.h"

// ProxyFactory skeleton

namespace CORBA {
namespace Nirvana {

template <class S>
class Skeleton <S, ProxyFactory>
{
public:
	static const typename Bridge <ProxyFactory>::EPV epv_;

protected:
	static Type <InterfaceMetadataPtr>::ABI_ret __get_metadata (Bridge <ProxyFactory>* obj, Interface* env)
	{
		try {
			return Type <InterfaceMetadataPtr>::ret (S::_implementation (obj).metadata ());
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return Type <InterfaceMetadataPtr>::ret ();
	}

	static Type <InterfacePtr>::ABI_ret _create_proxy (Bridge <ProxyFactory>* obj, Interface* target, UShort interface_idx, Type <DynamicServantPtr>::ABI_out deleter, Interface* env)
	{
		try {
			return Type <InterfacePtr>::ret (S::_implementation (obj).create_proxy (
				TypeItf <IOReference>::in (target), interface_idx,
				Type <DynamicServantPtr>::out (deleter)));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return TypeItf <Interface>::ret ();
	}
};

template <class S>
const Bridge <ProxyFactory>::EPV Skeleton <S, ProxyFactory>::epv_ = {
	{ // header
		Bridge <ProxyFactory>::repository_id_,
		S::template __duplicate <ProxyFactory>,
		S::template __release <ProxyFactory>
	},
	{ // epv
		&S::__get_metadata,
		S::_create_proxy
	}
};

/// Proxy factory implements ProxyFactory and TypeCode interfaces.
template <class S>
class ServantStatic <S, ProxyFactory> :
	public ImplementationPseudoStatic <S, ProxyFactory, TypeCode, AbstractBase>
{
public:
	Interface* _query_interface (const String& id)
	{
		return FindInterface <ProxyFactory, TypeCode>::find (*(S*)0, id);
	}
};

}
}

#endif
