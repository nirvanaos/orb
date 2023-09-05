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
#ifndef NIRVANA_ORB_DERIVEDSERVANT_H_
#define NIRVANA_ORB_DERIVEDSERVANT_H_
#pragma once

#include "Server.h"

namespace CORBA {

/// \brief Derived servant implementation (Nirvana extension).
/// 
/// Used to extend existing servant implementation with new interface(s).
/// 
/// \tparam S Servant class implementing operations.
/// \tparam BaseServant Base servant implementation.
/// \tparam Primary New primary interface.
/// \tparam Bases Additional interfaces.
template <class S, class BaseServant, class Primary, class ... Bases>
class DerivedServant :
	public BaseServant,
	public Internal::InterfaceImpl <S, Bases>...,
	public Internal::InterfaceImpl <S, Primary>,
	public Internal::ServantTraits <S>,
	public Internal::LifeCycleRefCnt <S>
{
	typedef Internal::ServantTraits <S> ServantTraits;
	typedef Internal::LifeCycleRefCnt <S> LifeCycle;

public:
	typedef Primary PrimaryInterface;

	using ServantTraits::_implementation;
	using ServantTraits::_wide_object;
	using ServantTraits::_wide;
	using LifeCycle::__duplicate;
	using LifeCycle::__release;
	using LifeCycle::_duplicate;
	using LifeCycle::_release;

	Internal::Interface* _query_interface (Internal::String_in id) noexcept
	{
		Internal::Interface* itf = Internal::FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
		if (itf)
			return itf;
		else
			return BaseServant::_query_interface (id);
	}

	Internal::I_ref <Primary> _this ()
	{
		return BaseServant::_get_proxy ().template downcast <Primary> ();
	}

protected:
	template <class ... Args>
	DerivedServant (Args ... args) :
		BaseServant (std::forward <Args> (args)...)
	{}
};

}

#endif
