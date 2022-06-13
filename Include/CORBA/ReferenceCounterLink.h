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
#ifndef NIRVANA_ORB_REFERENCECOUNTERLINK_H_
#define NIRVANA_ORB_REFERENCECOUNTERLINK_H_
#pragma once

#include "ObjectFactoryInc.h"
#include "ValueBase.h"
#include "primitive_types.h"

namespace CORBA {

template <class> class servant_reference;

namespace Internal {

template <class> class LifeCycleRefCnt;

class ReferenceCounterLink
{
public:

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class CORBA::Internal::LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
	// ??? template <class, class> friend class Skeleton;
#endif

	void _add_ref () const
	{
		core_object_->add_ref ();
	}

	void _remove_ref () const
	{
		core_object_->remove_ref ();
	}

	ULong _refcount_value () const
	{
		return core_object_->refcount_value ();
	}

protected:
	void _construct (Bridge <ValueBase>& vb);

private:
	I_ref <ReferenceCounter> core_object_;
};

}
}

#endif
