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
#ifndef NIRVANA_ORB_VALUEBASEIMPL_H_
#define NIRVANA_ORB_VALUEBASEIMPL_H_
#pragma once

#include "ValueImpl.h"
#include "LifeCycleRefCnt.h"
#include "RefCountBase.h"
#include "ValueBase_s.h"

namespace CORBA {
namespace Internal {

//! Implementation of ValueBase.
//! \tparam S Servant class implementing operations.
template <class S>
class ValueImpl <S, ValueBase> :
	public LifeCycleRefCnt <S>,
	public ServantTraits <S>,
	public ValueImplBase <S, ValueBase>,
	public RefCountBase <S>
{};

template <class S>
class ValueTraits
{
public:
	Type <ValueBase>::VRet _copy_value () const
	{
#ifndef LEGACY_CORBA_CPP
		return make_reference <S> (std::ref (static_cast <const S&> (*this)));
#else
		return new S (static_cast <const S&> (*this));
#endif
	}
};

}
}

#endif
