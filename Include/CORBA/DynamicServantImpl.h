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
#ifndef NIRVANA_ORB_DYNAMICSERVANTIMPL_H_
#define NIRVANA_ORB_DYNAMICSERVANTIMPL_H_
#pragma once

#include "LifeCycleServant.h"

namespace CORBA {
namespace Internal {

/// \brief Reference counting implementation for servant
template <class S>
class InterfaceImpl <S, DynamicServant> :
	public ServantTraits <S>,
	public LifeCycleServant <S>
{
public:
	/// \brief For the performance reasons, we don't implement 
	///        the ReferenceCounter interface and just return bridge to 
	///        the core implementation.
	///
	/// If you need to override `_add_ref()` and `_remove_ref()` operations
	/// for debugging purposes, you can add InterfaceImpl <S, ReferenceCounter>
	/// as class base and override operator Bridge <ReferenceCounter>&.
	operator Bridge <ReferenceCounter>& ()
	{
		return *&this->_reference_counter ();
	}
};

}
}

#endif
