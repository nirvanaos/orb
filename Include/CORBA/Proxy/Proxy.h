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
#ifndef NIRVANA_ORB_PROXY_H_
#define NIRVANA_ORB_PROXY_H_

#include "../Server.h"
#include "InterfaceMetadata.h"
#include "ProxyFactory_s.h"
#include "IOReference.h"
#include "IORequest.h"
#include "ProxyBase.h"
#include "TypeCodeString.h"
#include "TypeCodeSequence.h"
#include "TypeCodeException.h"
#include "TypeCodeEnum.h"
#include "TypeCodeStruct.h"
#include "TypeAlias.h"
#include "TypeCodeTypeDef.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Internal {

/// Proxy factory implements ProxyFactory and TypeCode interfaces.
template <class I> class ProxyFactoryImpl :
	public TypeCodeImpl <ServantStatic <ProxyFactoryImpl <I>, ProxyFactory>, TypeCodeWithId <Type <I>::tc_kind, I>, TypeCodeOps <I> >
{
public:
	// ProxyFactory
	static const InterfaceMetadata metadata_;

	static InterfaceMetadataPtr __get_metadata (Bridge <ProxyFactory>* obj, Interface* env)
	{
		return &metadata_;
	}

	Interface* create_proxy (
		I_ptr <IOReference> proxy_manager, UShort interface_idx,
		Interface*& deleter)
	{
		typedef Proxy <I> ProxyClass;
		typename std::aligned_storage <sizeof (ProxyClass), alignof (ProxyClass)>::type tmp;
		ObjectFactory::StatelessCreationFrame scb (&tmp, sizeof (ProxyClass), 0);
		g_object_factory->stateless_begin (scb);
		ProxyClass* proxy;
		try {
			new (&tmp) ProxyClass (proxy_manager, interface_idx);
			proxy = (ProxyClass*)g_object_factory->stateless_end (true);
		} catch (...) {
			g_object_factory->stateless_end (false);
			throw;
		}
		deleter = proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}

	// TypeCode
	static Type <String>::ABI_ret _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return const_string_ret (name_);
	}

	static const Char name_ [];
};

template <class I> struct ProxyTraits;

}
}

#define IMPLEMENT_PROXY_FACTORY(ns, I) template <> const Char ProxyFactoryImpl <ns I>::name_ [] = #I

#endif
