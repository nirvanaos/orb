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
#ifndef NIRVANA_ORB_SERVANTIMPL_H_
#define NIRVANA_ORB_SERVANTIMPL_H_
#pragma once

#include "Exception.h"
#include "RepId.h"
#include "Bridge.h"
#include <Nirvana/throw_exception.h>

namespace CORBA {

class Object;
class LocalObject;
class AbstractBase;

namespace Internal {

extern void _check_pointer (const void* p);
extern void _check_pointer (const Interface* obj, const Interface::EPV& epv);

template <class S, class I> class Skeleton;

//! Standard servant mix-in.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Primary interface.
template <class S, class I> class Servant;

//! Standard (dynamic) servant traits.
//! \tparam S Servant class, derived from this.
template <class S>
class ServantTraits
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::epv_.header);
		return static_cast <S&> (*bridge);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, String_in id, Interface* env)
	{
		try {
			if (!RepId::compatible (Bridge <Base>::repository_id_, id))
				::Nirvana::throw_INV_OBJREF ();
			return &static_cast <Bridge <Base>&> (S::_implementation (derived));
		} catch (Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}

	template <class Derived>
	static Bridge <Object>* _wide_object (Bridge <Derived>* derived, String_in id, Interface* env)
	{
		return nullptr;
	}

	template <>
	static Bridge <Object>* _wide_object (Bridge <LocalObject>* derived, String_in id, Interface* env)
	{
		return S::_implementation (derived)._get_object (id, env);
	}

	template <class Derived>
	static Bridge <AbstractBase>* _wide_abstract (Bridge <Derived>* derived, String_in id, Interface* env)
	{
		return S::_implementation (derived)._get_abstract_base (id, env);
	}
};

//! Standard interface implementation.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Interface.
template <class S, class I>
class InterfaceImplBase :
	public Bridge <I>,
	public Skeleton <S, I>
{
protected:
	InterfaceImplBase () :
		Bridge <I> (Skeleton <S, I>::epv_)
	{}

	InterfaceImplBase (const InterfaceImplBase&) :
		InterfaceImplBase ()
	{}
};

template <class S, class I>
class InterfaceImpl :
	public InterfaceImplBase <S, I>
{};

//! Standard valuetype implementation.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Interface.
template <class S, class I>
class ValueImplBase :
	public BridgeVal <I>,
	public Skeleton <S, I>
{
protected:
	ValueImplBase () :
		BridgeVal <I> (Skeleton <S, I>::epv_)
	{}

	ValueImplBase (const ValueImplBase&) :
		ValueImplBase ()
	{}
};

template <class S, class I>
class ValueImpl :
	public ValueImplBase <S, I>
{};

}
}

#endif
