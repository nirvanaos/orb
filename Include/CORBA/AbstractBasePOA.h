/// \file AbstractBasePOA.h
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
#ifndef NIRVANA_ORB_ABSTRACTBASEPOA_H_
#define NIRVANA_ORB_ABSTRACTBASEPOA_H_

#include "AbstractBase_s.h"
#include "ReferenceCounterPOA.h"

namespace CORBA {
namespace Nirvana {

//! POA implementation of AbstractBase

template <>
class NIRVANA_NOVTABLE ServantPOA <AbstractBase> :
	public ServantPOA <ReferenceCounter>,
	public InterfaceImplBase <ServantPOA <AbstractBase>, AbstractBase>
{
public:
	virtual Interface_ptr _query_interface (const String& id) = 0;

protected:
	ServantPOA ()
	{}
};

}
}

#endif
