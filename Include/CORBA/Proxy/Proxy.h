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
#include "TypeCodeString.h"
#include "TypeCodeSequence.h"
#include "TypeCodeException.h"
#include "TypeCodeEnum.h"
#include "TypeCodeStruct.h"
#include "TypeAlias.h"
#include "TypeCodeTypeDef.h"
#include "TypeCodeInterface.h"
#include "TypeCodeValue.h"
#include "TypeCodeValueBox.h"
#include "TypeCodeNative.h"
#include "TypeCodeArray.h"
#include "ValueFactory.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Internal {

/// Proxy factory implements PseudoBase, ProxyFactory and TypeCode interfaces.
template <class I>
class ProxyFactoryImpl :
	public TypeCodeInterface <I>,
	public InterfaceStaticBase <ProxyFactoryImpl <I>, PseudoBase>,
	public InterfaceStaticBase <ProxyFactoryImpl <I>, ProxyFactory>,
	public ServantTraitsStatic <ProxyFactoryImpl <I> >
{
public:
	using ServantTraitsStatic <ProxyFactoryImpl <I> >::_implementation;

	// PseudoBase
	static Interface* _query_interface (String_in id)
	{
		return FindInterface <ProxyFactory, TypeCode>::find (*(ProxyFactoryImpl <I>*)nullptr, id);
	}

	// ProxyFactory
	static const InterfaceMetadata metadata_;

	static InterfaceMetadataPtr __get_metadata (Bridge <ProxyFactory>* obj, 
		Interface* env)
	{
		return &metadata_;
	}

	Interface* create_proxy (
		IOReference::_ptr_type proxy_manager, UShort interface_idx,
		Interface*& deleter)
	{
		typedef Proxy <I> ProxyClass;
		ProxyClass* proxy;

		if (g_object_factory->stateless_available ()) {
			typename std::aligned_storage <sizeof (ProxyClass), 
				alignof (ProxyClass)>::type tmp;
			ObjectFactory::StatelessCreationFrame scb (&tmp, sizeof (ProxyClass), 0, nullptr);
			g_object_factory->stateless_begin (scb);
			try {
				new (&tmp) ProxyClass (proxy_manager, interface_idx);
				proxy = (ProxyClass*)g_object_factory->stateless_end (true);
			} catch (...) {
				g_object_factory->stateless_end (false);
				throw;
			}
		} else
			proxy = new ProxyClass (proxy_manager, interface_idx);
		deleter = proxy->_dynamic_servant ();
		return proxy->_proxy ();
	}

};

}
}

#endif
