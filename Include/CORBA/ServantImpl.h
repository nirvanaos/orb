/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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

#include "Exception.h"
#include "RepositoryId.h"
#include "I_ptr.h"

namespace CORBA {

class Object;

namespace Nirvana {

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
			if (!RepositoryId::compatible (Bridge <Base>::repository_id_, id))
				::Nirvana::throw_INV_OBJREF ();
			return &static_cast <Bridge <Base>&> (S::_implementation (derived));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}

	template <class Derived>
	static Bridge <Object>* _wide_object (Bridge <Derived>* derived, String_in id, Interface* env)
	{
		try {
			return S::_implementation (derived)._get_object (id);
		} catch (const Exception & e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
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

}
}

#endif
