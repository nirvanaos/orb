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

#include "../CORBA.h"
#include "../FindInterface.h"
#include "ProxyFactory_s.h"
#include "IOReference.h"
#include "IORequest.h"
#include "ProxyBase.h"
#include "TypeCodeString.h"
#include "TypeCodeSequence.h"
#include "TypeCodeException.h"
#include "TypeCodeEnum.h"
#include "TypeCodeStruct.h"
#include "TypeCodeTypeDef.h"
#include <Nirvana/stateless.h>
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {

/// Proxy factory implements ProxyFactory and TypeCode interfaces.
template <class I> class ProxyFactoryImpl :
	public TypeCodeImpl <ServantStatic <ProxyFactoryImpl <I>, ProxyFactory>, TypeCodeWithId <Type <I_var <I> >::tc_kind, I>, TypeCodeOps <I_var <I> > >
{
public:
	// ProxyFactory
	static const InterfaceMetadata metadata_;

	Interface_ptr create_proxy (
		IOReference_ptr proxy_manager, UShort interface_idx,
		DynamicServant_var& deleter)
	{
		Proxy <I>* proxy =
			::Nirvana::stateless_create <Proxy <I> > (proxy_manager, interface_idx);
		deleter = proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}

	// TypeCode
	static const char* _name (Bridge <TypeCode>* _b, Interface* _env)
	{
		return name_;
	}

	static const Char name_ [];
};

template <class I> struct ProxyTraits;

}
}

#define IMPLEMENT_PROXY_FACTORY(ns, I) template <> const Char ProxyFactoryImpl <ns::I>::name_ [] = #I

#endif
