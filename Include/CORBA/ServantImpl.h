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
#include "String.h"
#include "set_exception.h"

namespace CORBA {

class Object;
class LocalObject;
class AbstractBase;

namespace Internal {

extern void check_pointer (const void* p);
extern void check_pointer (const Interface* obj, const Interface::EPV& epv);
extern void check_pointer_noexcept (const Interface* obj, const Interface::EPV& epv,
	Interface* env) noexcept;

template <class S, class Itf> class Skeleton;

//! Standard servant mix-in.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam Itf Primary interface.
template <class S, class Itf> class Servant;

//! Standard (dynamic) servant traits.
//! \tparam S Servant class, derived from this.
template <class S>
class ServantTraits
{
public:
	template <class Itf>
	static S& _implementation (Bridge <Itf>* bridge)
	{
		check_pointer (bridge, Skeleton <S, Itf>::epv_.header);
		return static_cast <S&> (*bridge);
	}

	template <class Itf>
	static const S& _implementation (const Bridge <Itf>* bridge)
	{
		return S::_implementation (const_cast <Bridge <Itf>*> (bridge));
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>*, Type <String>::ABI_in,
		Interface*) noexcept
	{
		// It really needed only for value types.
		assert (false);
		return nullptr;
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide_val (Bridge <Derived>* derived, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		if (!RepId::compatible (RepIdOf <Base>::id, Type <String>::in (id)))
			set_INV_OBJREF (env);
		return &static_cast <Bridge <Base>&> (S::_implementation (derived));
	}

	template <class Derived>
	static Bridge <Object>* _wide_object (Bridge <Derived>* derived, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		return S::_implementation (derived)._get_object (id, env);
	}

	template <class Derived>
	static Bridge <AbstractBase>* _wide_abstract (Bridge <Derived>* derived, Type <String>::ABI_in id,
		Interface* env) noexcept
	{
		return S::_implementation (derived)._get_abstract_base (id, env);
	}
};

//! Standard interface implementation.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam Itf Interface.
template <class S, class Itf>
class InterfaceImplBase :
	public Bridge <Itf>,
	public Skeleton <S, Itf>
{
protected:
	InterfaceImplBase () :
		Bridge <Itf> (Skeleton <S, Itf>::epv_)
	{}

	InterfaceImplBase (const InterfaceImplBase&) :
		InterfaceImplBase ()
	{}
};

template <class S, class Itf>
class InterfaceImpl :
	public InterfaceImplBase <S, Itf>
{};

}

template <class> struct servant_traits;

}

#endif
