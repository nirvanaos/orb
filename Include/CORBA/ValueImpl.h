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
#ifndef NIRVANA_ORB_VALUEIMPL_H_
#define NIRVANA_ORB_VALUEIMPL_H_
#pragma once

#include "ServantImpl.h"
#include "ValueBaseImpl.h"
#include "servant_reference.h"
#include "IORequestClient.h"

namespace CORBA {
namespace Internal {

/// Standard valuetype implementation.
/// \tparam S Servant class implementing operations. Must derive from this mix-in.
/// \tparam I Value interface.
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

/// Default (stateless) value implementation.
///
/// \tparam S Servant class implementing operations.
/// \tparam I Value interface.
template <class S, class I>
class ValueImpl :
	public ValueImplBase <S, I>
{};

/// Value state data.
///
/// \tparam I Value interface.
template <class I> class ValueData;

/// Non truncatable value
class ValueNonTruncatable
{
public:
	static Interface* __truncatable_base (Bridge <ValueBase>*, Interface*);
};

/// Truncatable value
///
/// \typeparam tc Base type code.
template <Nirvana::ImportInterfaceT <TypeCode>* tc>
class ValueTruncatable
{
public:
	static Interface* __truncatable_base (Bridge <ValueBase>*, Interface*)
	{
		return &I_ptr <TypeCode> (*tc);
	}
};

/// Abstract value mix-in
class ValueAbstract : public ValueNonTruncatable
{
public:
	static Interface* __copy_value (Bridge <ValueBase>*, Interface*);
	static Interface* __factory (Bridge <ValueBase>*, Interface*);
	static void __marshal (Bridge <ValueBase>*, Interface*, Interface*);
	static void __unmarshal (Bridge <ValueBase>*, Interface*, Interface*);
};

/// Abstract value base
template <class S>
class ValueBaseAbstract :
	public ValueImpl <S, ValueBase>,
	public ValueAbstract
{
public:
	using ValueAbstract::__copy_value;
	using ValueAbstract::__factory;
	using ValueAbstract::__marshal;
	using ValueAbstract::__unmarshal;
	using ValueAbstract::__truncatable_base;
};

template <Nirvana::ImportInterfaceT <TypeCode>* truncatable_base>
using TruncatableBase = typename std::conditional <truncatable_base == nullptr,
	ValueNonTruncatable, ValueTruncatable <truncatable_base> >::type;

template <class I>
I_ptr <CORBA::ValueFactoryBase> get_factory () noexcept;

/// Concrete value base
template <class Base, class S, class I, Nirvana::ImportInterfaceT <TypeCode>* truncatable_base>
class ValueConcrete :
	public Base,
	public TruncatableBase <truncatable_base>
{
public:
	using TruncatableBase <truncatable_base>::__truncatable_base;

	Type <ValueBase>::VRet _copy_value () const
	{
#ifndef LEGACY_CORBA_CPP
		return make_reference <S> (std::ref (static_cast <const S&> (*this)));
#else
		return new S (static_cast <const S&> (*this));
#endif
	}

	static I_ptr <ValueFactoryBase> _factory () noexcept
	{
		return get_factory <I> ();
	}
};

/// Concrete value base
template <class S, class I, Nirvana::ImportInterfaceT <TypeCode>* truncatable_base = nullptr>
using ValueBaseConcrete = ValueConcrete <ValueImpl <S, ValueBase>, S, I, truncatable_base>;

/// Value base with interface support
template <class S, class I, Nirvana::ImportInterfaceT <TypeCode>* truncatable_base = nullptr>
using ValueBaseSupports = ValueConcrete <ValueImplBase <S, ValueBase>, S, I, truncatable_base>;

}
}

#endif
