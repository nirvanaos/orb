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
#ifndef NIRVANA_ORB_VALUEBASESTATIC_H_
#define NIRVANA_ORB_VALUEBASESTATIC_H_
#pragma once

#include "ServantStatic.h"
#include "LifeCycleStatic.h"
#include "ValueBase_s.h"
#include "ValueImpl.h"

namespace CORBA {
namespace Internal {

template <class S>
class InterfaceStatic <S, ValueBase> :
	public InterfaceStaticBase <S, ValueBase>,
	public ValueAbstract
{
public:
	using ValueAbstract::__copy_value;
	using ValueAbstract::__factory;
	using ValueAbstract::__marshal;
	using ValueAbstract::__unmarshal;
	using ValueAbstract::__truncatable_base;

	// For AbstractBase
	static Type <ValueBase>::VRet _to_value () noexcept
	{
#ifndef LEGACY_CORBA_CPP
		return InterfaceStaticBase <S, ValueBase>::_bridge ();
#else
		return ValueBase::_duplicate (InterfaceStaticBase <S, ValueBase>::_bridge ());
#endif
	}

};

}
}

#endif

