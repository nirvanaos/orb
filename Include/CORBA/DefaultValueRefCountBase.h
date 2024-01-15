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
#ifndef NIRVANA_ORB_DEFAULTVALUEREFCOUNTBASE_H_
#define NIRVANA_ORB_DEFAULTVALUEREFCOUNTBASE_H_
#pragma once

#include "ValueBasePOA.h"
#include "RefCountBase.h"

namespace CORBA {

class NIRVANA_NOVTABLE DefaultValueRefCountBase :
	public virtual Internal::ServantPOA <ValueBase>,
	public Internal::RefCountBase <DefaultValueRefCountBase>
{
	typedef Internal::RefCountBase <DefaultValueRefCountBase> Base;

public:

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
	template <class> friend class CORBA::servant_reference;
#endif
	virtual void _add_ref () override
	{
		Base::_add_ref ();
	}

	virtual void _remove_ref () override
	{
		Base::_remove_ref ();
	}

public:
	virtual ULong _refcount_value () override
	{
		return Base::_refcount_value ();
	}

};

}

#endif
