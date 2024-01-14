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
#ifndef NIRVANA_ORB_VALUEBASEPOA_H_
#define NIRVANA_ORB_VALUEBASEPOA_H_
#pragma once

#include "AbstractBasePOA.h"
#include "ValueImpl.h"
#include "ValueFactoryBase.h"
#include "ValueBase_s.h"
#include "LifeCycleRefCnt.h"

namespace CORBA {

template <class> class servant_reference;

namespace Internal {

template <>
class NIRVANA_NOVTABLE ServantPOA <ValueBase> :
	public ValueImplBase <ServantPOA <ValueBase>, ValueBase>,
	public virtual ServantPOA <AbstractBase>
{
public:
	virtual Type <ValueBase>::VRet _copy_value () const
	{
		return nullptr;
	}

	virtual void _marshal (I_ptr <IORequest> rq) const;
	virtual void _unmarshal (I_ptr <IORequest> rq);
	virtual Interface* _query_valuetype (const String& id) noexcept = 0;

	virtual Type <TypeCode>::VRet _truncatable_base () const noexcept
	{
		return nullptr;
	}

	virtual Type <ValueFactoryBase>::VRet _factory () const noexcept = 0;

protected:
	ServantPOA ()
	{}

	virtual ~ServantPOA ()
	{}
};

}
}

#endif
