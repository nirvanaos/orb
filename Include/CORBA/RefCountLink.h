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
#ifndef NIRVANA_ORB_REFCOUNTLINK_H_
#define NIRVANA_ORB_REFCOUNTLINK_H_
#pragma once

#include "ValueMemory.h"
#include "DynamicServantImpl.h"
#include "RefCnt.h"

namespace CORBA {
namespace Internal {

template <class S>
class LifeCycleRefCnt;

class RefCountLink :
	public DynamicServantBridge,
	public ValueMemory
{
#ifdef LEGACY_CORBA_CPP
public:
#else
protected:
	template <class T, class ... Args>
	friend CORBA::servant_reference <T> CORBA::make_reference (Args&& ... args);

	template <class T, class ... Args>
	friend CORBA::Internal::I_ref <typename T::PrimaryInterface> CORBA::make_pseudo (Args&& ... args);

	template <class> friend class CORBA::servant_reference;
	template <class> friend class LifeCycleRefCnt;
	template <class, class> friend class Skeleton;
#endif

	void _add_ref () const
	{
		core_object_->add_ref ();
	}

	void _remove_ref () const
	{
		core_object_->remove_ref ();
	}

public:
	ULong _refcount_value () const
	{
		return core_object_->refcount_value ();
	}

protected:
	RefCountLink (const Bridge <DynamicServant>::EPV& epv);
	RefCountLink (const RefCountLink& src);

	~RefCountLink ()
	{}

	RefCountLink& operator = (const RefCountLink&) noexcept
	{
		return *this;
	}

private:
	I_ref <RefCnt> core_object_;
};

}
}

#endif
