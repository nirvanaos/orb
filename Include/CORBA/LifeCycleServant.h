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
#ifndef NIRVANA_ORB_LIFECYCLESERVANT_H_
#define NIRVANA_ORB_LIFECYCLESERVANT_H_

#include "ReferenceCounter.h"
#include "DynamicServant_s.h"
#include "ServantImpl.h"
#include "ServantMemory.h"
#include "LifeCycleRefCnt.h"

namespace CORBA {
namespace Nirvana {

/// \class ReferenceCounterLink
///
/// \brief Delegates reference counting to core
///        implementation of the ReferenceCounter interface.
class ReferenceCounterLink :
	public ServantMemory,
	public Bridge <DynamicServant>
{
	ReferenceCounterLink (const ReferenceCounterLink&) = delete;
public:
	void _add_ref ()
	{
		reference_counter_->_add_ref ();
	}

	void _remove_ref ()
	{
		reference_counter_->_remove_ref ();
	}

	ULong _refcount_value ()
	{
		return reference_counter_->_refcount_value ();
	}

protected:
	ReferenceCounterLink (const Bridge <DynamicServant>::EPV& epv);

	ReferenceCounterLink& operator = (const ReferenceCounterLink&)
	{
		return *this; // Do nothing
	}

protected:
	ReferenceCounter_ptr _reference_counter () const
	{
		return reference_counter_;
	}

private:
	ReferenceCounter_var reference_counter_;
};

/// \brief Dynamic servant life cycle implementation
/// \tparam S Servant class
///
/// We don't implement reference counting in the object implementation
/// because implementation of the stateless objects resides in the
/// read-only memory and can't be changed.
/// So reference counting for objects implemented at the core.
template <class S>
class LifeCycleServant :
	public Skeleton <S, DynamicServant>,
	public ReferenceCounterLink,
	public LifeCycleRefCnt <S>
{
protected:
	LifeCycleServant () :
		ReferenceCounterLink (Skeleton <S, DynamicServant>::epv_)
	{}
};

}
}

#endif
