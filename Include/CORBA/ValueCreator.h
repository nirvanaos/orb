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
#include "ValueImpl.h"
#include <type_traits>
#include <utility>

namespace CORBA {
namespace Internal {

template <class S, class I> class ValueCreator;

template <class T>
class ImplCopy final : public T
{
public:
	template <class ... Args>
	ImplCopy (Args ... args) :
		T (std::forward <Args> (args)...)
	{}

private:
	virtual Type <ValueBase>::VRet _copy_value () const override
	{
		return ::CORBA::Internal::copy_value (*this);
	}
};

template <class T>
class ImplRefCnt final :
	public T,
	public DefaultValueRefCountBase
{
public:
	template <class ... Args>
	ImplRefCnt (Args ... args) :
		T (std::forward <Args> (args)...)
	{}

};

template <class T>
class ImplRefCntCopy final :
	public T,
	public DefaultValueRefCountBase
{
public:
	template <class ... Args>
	ImplRefCntCopy (Args ... args) :
		T (std::forward <Args> (args)...)
	{}

private:
	virtual Type <ValueBase>::VRet _copy_value () const override
	{
		return ::CORBA::Internal::copy_value (*this);
	}
};

template <class Impl>
class ValueCreatorBase
{
public:
	using ImplType = typename std::conditional <std::is_base_of <ServantPOA <AbstractBase>, Impl>::value,
		typename std::conditional <IDL::traits <typename Impl::PrimaryInterface>::is_abstract::value,
		typename std::conditional <std::is_abstract <Impl>::value, ImplRefCnt <Impl>, Impl>::type,
		typename std::conditional <std::is_abstract <Impl>::value, ImplRefCntCopy <Impl>, ImplCopy <Impl> >::type>::type,
		Impl>::type;

	static I_ref <ValueBase> create_for_unmarshal ()
	{
		return make_reference <ImplType> ();
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
