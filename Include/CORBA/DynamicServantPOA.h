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
#ifndef NIRVANA_ORB_LIFECYCLEPOA_H_
#define NIRVANA_ORB_LIFECYCLEPOA_H_

#include "LifeCycleServant.h"
#include "ServantPOA.h"

namespace CORBA {
namespace Nirvana {

template <>
class NIRVANA_NOVTABLE ServantPOA <DynamicServant> :
	public ServantTraitsPOA,
	public LifeCycleServant <ServantPOA <DynamicServant> >
{
public:
	virtual ~ServantPOA ()
	{}

	virtual void _add_ref ()
	{
		LifeCycleServant <ServantPOA <DynamicServant> >::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		LifeCycleServant <ServantPOA <DynamicServant> >::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return LifeCycleServant <ServantPOA <DynamicServant> >::_refcount_value ();
	}

protected:
	ServantPOA ()
	{}
};

}
}

#endif
