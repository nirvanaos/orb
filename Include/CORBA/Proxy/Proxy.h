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
#include "../IORequestClient.h"
#include "ProxyBase.h"
#include "TypeCodeException.h"
#include "TypeCodeEnum.h"
#include "TypeCodeStruct.h"
#include "TypeCodeUnion.h"
#include "TypeCodeInterface.h"
#include "TypeCodeValue.h"
#include "TypeCodeValueBox.h"
#include "TypeCodeNative.h"
#include "ValueFactoryImpl.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Internal {

/// Proxy factory implements PseudoBase, ProxyFactory and TypeCode interfaces.
template <class S, class I>
class ProxyFactoryBase :
	public TypeCodeInterface <I>,
	public InterfaceStaticBase <S, PseudoBase>,
	public InterfaceStaticBase <S, ProxyFactory>,
	public ServantTraitsStatic <S>
{
public:
	using ServantTraitsStatic <S>::_implementation;

	// PseudoBase
	static Interface* _query_interface (String_in id)
	{
		return FindInterface <ProxyFactory, TypeCode>::find (*(S*)nullptr, id);
	}

	// ProxyFactory

	static InterfaceMetadataPtr _s__get_metadata (Bridge <ProxyFactory>* obj,
		Interface* env)
	{
		return &MetadataOf <I>::metadata_;
	}

};

template <class I>
class ProxyFactoryImpl :
	public ProxyFactoryBase <ProxyFactoryImpl <I>, I>
{
public:
	Interface* create_proxy (
		IOReference::_ptr_type proxy_manager, UShort interface_idx, Interface*& servant,
		Interface*& deleter)
	{
		typedef Proxy <I> ProxyClass;
		ProxyClass* proxy;

		typename std::aligned_storage <sizeof (ProxyClass), 
			alignof (ProxyClass)>::type tmp;
		ObjectFactory::StatelessCreationFrame scb (&tmp, sizeof (ProxyClass), 0, nullptr);
		g_object_factory->stateless_begin (scb);
		try {
			new (&tmp) ProxyClass (proxy_manager, interface_idx, servant);
			proxy = (ProxyClass*)g_object_factory->stateless_end (true);
		} catch (...) {
			g_object_factory->stateless_end (false);
			throw;
		}
		deleter = &proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}

	Interface* create_poller (::Messaging::Poller::_ptr_type aggregate, Interface*& deleter)
	{
		throw NO_IMPLEMENT ();
	}
};

}
}

#endif
