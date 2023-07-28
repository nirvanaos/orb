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
#ifndef NIRVANA_ORB_SERVANTBASEIMPL_H_
#define NIRVANA_ORB_SERVANTBASEIMPL_H_
#pragma once

#include "ServantBaseLink.h"
#include "ServantBase_s.h"
#include "ServantMemory.h"

namespace CORBA {
namespace Internal {

//! Implementation of PortableServer::ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class ValueImpl <S, PortableServer::ServantBase> :
	public ServantBaseLink,
	public Skeleton <S, PortableServer::ServantBase>,
	public LifeCycleRefCnt <S>,
	public ServantTraits <S>,
	public ServantMemory
{
public:
	void _delete_object () noexcept
	{
		delete& static_cast <S&> (*this);
	}

	static Bridge <AbstractBase>* _get_abstract_base (Type <String>::ABI_in iid,
		Interface* env) noexcept
	{
		assert (false);
		return nullptr;
	}

protected:
	ValueImpl () :
		ServantBaseLink (Skeleton <S, PortableServer::ServantBase>::epv_)
	{}

	ValueImpl (const ValueImpl&) :
		ValueImpl ()
	{}

	ValueImpl& operator = (const ValueImpl&) noexcept
	{
		return *this; // Do nothing
	}

};

}
}

#endif
