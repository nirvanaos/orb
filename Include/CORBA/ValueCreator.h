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
#ifndef NIRVANA_ORB_VALUECREATOR_H_
#define NIRVANA_ORB_VALUECREATOR_H_
#pragma once

#include "ImplementationPseudoStatic.h"
#include "ValueFactoryBase.h"

namespace CORBA {
namespace Internal {

template <class S, class I> class ValueCreator;

template <class Impl>
class ValueCreatorBase
{
public:
	static I_ref <ValueBase> create_for_unmarshal ()
	{
		return make_reference <Impl> ();
	}

};

template <class Impl>
class ValueCreatorNoFactory :
	public ImplementationPseudoStatic <ValueCreatorNoFactory <Impl>, ValueFactoryBase>,
	public ValueCreatorBase <Impl>
{
public:
	
	static I_ptr <Interface> _query_factory (String_in id) noexcept
	{
		return nullptr;
	}

};

#if defined (_MSC_VER) && !defined (__clang__)
#pragma warning (push)
#pragma warning (disable: 4584)
#endif

template <class S, class Factory>
class ValueCreatorImpl :
	public ImplementationPseudoStatic <S, ValueFactoryBase>,
	public InterfaceStaticBase <S, Factory>
{
public:
	static I_ptr <Interface> _query_factory (String_in id)
	{
		if (RepId::compatible (RepIdOf <Factory>::id, id))
			return InterfaceStaticBase <S, Factory>::_bridge ();
		return nullptr;
	}

};

#if defined (_MSC_VER) && !defined (__clang__)
#pragma warning (pop)
#endif

/// This function returns ValueFactoryBase for the specific value type.
/// 
/// \typeparam I Value type interface
/// \returns Pointer to ValueFactoryBase without the incrementing reference counter.
template <class I>
CORBA::ValueFactoryBase::_ptr_type get_factory () noexcept;

}
}

#define NIRVANA_VALUETYPE_IMPL(V, Impl) template <> CORBA::ValueFactoryBase::_ptr_type CORBA::Internal::get_factory <V> () noexcept {\
	return CORBA::Internal::ValueCreator <Impl, V>::_get_ptr (); }

#endif
