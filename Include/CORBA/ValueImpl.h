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
#include "ValueBase.h"
#include "servant_reference.h"

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

class ValueBaseNoCopy
{
public:
	static Interface* __copy_value (Bridge <ValueBase>*, Interface*);
};

template <class S>
class ValueBaseCopy
{
public:
	static Interface* __copy_value (Bridge <ValueBase>* _b, Interface* _env)
	{
		try {
			return Type <ValueBase>::ret (S::_implementation (_b)._copy_value ());
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
		return Type <ValueBase>::ret ();
	}

	Type <ValueBase>::VRet _copy_value () const
	{
#ifndef LEGACY_CORBA_CPP
		return make_reference <S> (std::ref (static_cast <const S&> (*this)));
#else
		return new S (static_cast <const S&> (*this));
#endif
	}
};

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
		return interface_duplicate (&TypeCode::_ptr_type (*tc));
	}
};

/// Abstract value without factory
class ValueBaseNoFactory
{
public:
	static Interface* __factory (Bridge <ValueBase>* _b, Interface* _env);
	static void __marshal (Bridge <ValueBase>*, Interface*, Interface* _env);
	static void __unmarshal (Bridge <ValueBase>*, Interface*, Interface* _env);
};

/// Concrete value with factory.
/// 
/// \tparam I Value interface.
template <class I>
class ValueBaseFactory
{
public:
	static Interface* __factory (Bridge <ValueBase>* _b, Interface* _env) noexcept
	{
		return _factory_base ();
	}

	static Interface* _factory_base () noexcept
	{
		return interface_duplicate (query_creator_interface <I> (RepIdOf <ValueFactoryBase>::id));
	}
};

/// Concrete value with marshaling.
/// 
/// \tparam S Servant class implementing operations. Must derive from this mix-in.
template <class S>
class ValueBaseMarshal
{
public:
	static void __marshal (Bridge <ValueBase>* _b, Interface* rq, Interface* _env)
	{
		try {
			S::_implementation (_b)._marshal (Type <IORequest>::in (rq));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}

	static void __unmarshal (Bridge <ValueBase>* _b, Interface* rq, Interface* _env)
	{
		try {
			S::_implementation (_b)._unmarshal (Type <IORequest>::in (rq));
		} catch (Exception& e) {
			set_exception (_env, e);
		} catch (...) {
			set_unknown_exception (_env);
		}
	}
};

}
}

#endif
