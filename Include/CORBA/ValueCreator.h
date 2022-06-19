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
	public InterfaceStaticBase <ValueCreatorNoFactory <Impl>, ValueFactoryBase>,
	public ServantTraitsStatic <ValueCreatorNoFactory <Impl> >,
	public LifeCycleStatic,
	public ValueCreatorBase <Impl>
{
public:
	static Interface* _query_factory (String_in id) NIRVANA_NOEXCEPT
	{
		return nullptr;
	}

	static Interface* _query_interface (String_in id)
	{
		if (RepId::compatible (ValueFactoryBase::repository_id_, id))
			return InterfaceStaticBase <ValueCreatorNoFactory <Impl>, ValueFactoryBase>::_bridge ();
		return _query_factory (id);
	}

};

template <class S, class Factory>
class ValueFactoryImpl :
	public InterfaceStaticBase <S, ValueFactoryBase>,
	public InterfaceStaticBase <S, Factory>,
	public ServantTraitsStatic <S>,
	public LifeCycleStatic
{
public:
	static Interface* _query_factory (String_in id)
	{
		if (RepId::compatible (RepIdOf <Factory>::repository_id_, id))
			return InterfaceStaticBase <S, Factory>::_bridge ();
		return nullptr;
	}

	static Interface* _query_interface (String_in id)
	{
		if (RepId::compatible (RepIdOf <ValueFactoryBase>::repository_id_, id))
			return InterfaceStaticBase <S, ValueFactoryBase>::_bridge ();
		return _query_factory (id);
	}
};

}
}

#define NIRVANA_VALUETYPE_IMPL(V, Impl) template <>\
CORBA::Internal::Interface* CORBA::Internal::query_creator_interface <V> (String_in id)\
{ return ValueCreator <Impl, V>::_query_interface (id); }

#endif
