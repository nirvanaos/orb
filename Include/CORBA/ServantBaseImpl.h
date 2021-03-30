/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_ORB_SERVANTBASEIMPL_H_
#define NIRVANA_ORB_SERVANTBASEIMPL_H_

#include "AbstractBaseImpl.h"
#include "ServantBaseLink.h"
#include "ServantBase_s.h"

namespace CORBA {
namespace Nirvana {

//! Implementation of PortableServer::ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, PortableServer::ServantBase> :
	public InterfaceImpl <S, AbstractBase>,
	public Skeleton <S, PortableServer::ServantBase>,
	public ServantBaseLink
{
protected:
	InterfaceImpl () :
		ServantBaseLink (Skeleton <S, PortableServer::ServantBase>::epv_)
	{
		_construct ();
	}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}
};

}
}

#endif
