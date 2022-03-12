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
#include "ReferenceCounterLink.h"
#include "ValueBase_s.h"

namespace CORBA {
namespace Internal {

template <>
class NIRVANA_NOVTABLE ServantPOA <ValueBase> :
	public ReferenceCounterLink,
	public virtual ServantPOA <AbstractBase>,
	public Skeleton <ServantPOA <ValueBase>, ValueBase>
{
public:
	virtual I_ref <ValueBase> _copy_value () = 0;
	virtual void _marshal (I_ptr <IORequest> rq) = 0;
	virtual void _unmarshal (I_ptr <IORequest> rq) = 0;

#ifndef LEGACY_CORBA_CPP
protected:
	template <class> friend class LifeCycleRefCnt;
	template <class> friend class servant_reference;
	friend class Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>;
#endif

	virtual void _add_ref ()
	{
		ReferenceCounterLink::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		ReferenceCounterLink::_remove_ref ();
	}

protected:
	ServantPOA () :
		ReferenceCounterLink (Skeleton <ServantPOA <ValueBase>, ValueBase>::epv_)
	{}
};

}
}

#endif
