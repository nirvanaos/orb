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
#ifndef NIRVANA_ORB_LOCALOBJECTIMPL_H_
#define NIRVANA_ORB_LOCALOBJECTIMPL_H_
#pragma once

#include "AbstractBaseImpl.h"
#include "LocalObjectLink.h"
#include "LocalObject_s.h"
#include "ServantMemory.h"

namespace CORBA {
namespace Internal {

//! \brief Implementation of CORBA::LocalObject
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, LocalObject> :
	public LocalObjectLink,
	public Skeleton <S, LocalObject>,
	public InterfaceImpl <S, AbstractBase>,
	public ServantMemory
{
public:
	void _delete_object () NIRVANA_NOEXCEPT
	{
		delete& static_cast <S&> (*this);
	}

protected:
	InterfaceImpl () :
		LocalObjectLink (Skeleton <S, LocalObject>::epv_)
	{
		_construct (*this);
	}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}

};

}
}

#endif
