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
#ifndef NIRVANA_ORB_IMPLEMENTATIONLOCALPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONLOCALPOA_H_

#include "LocalObjectPOA.h"
#include "FindInterface.h"

namespace CORBA {
namespace Internal {

//! \class ImplementationLocalPOA
//!
//! \brief Portable implementation of localinterface.
//!
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class Primary, class ... Bases>
class NIRVANA_NOVTABLE ImplementationLocalPOA :
	public virtual ServantPOA <LocalObject>,
	public virtual ServantPOA <Bases>...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	typedef Primary PrimaryInterface;

	virtual Interface* _query_interface (const String& id)
	{
#ifdef _DEBUG
		Bridge <AbstractBase>* ab = this;
		const Bridge <AbstractBase>::EPV& epv = ab->_epv ();
		assert (!strcmp (epv.header.interface_id, Bridge <AbstractBase>::repository_id_));
#endif
		return FindInterface <Primary, Bases...>::find (static_cast <ServantPOA <Primary>&> (*this), id);
	}

	I_ref <Primary> _this ()
	{
		return this->_get_proxy ().template downcast <Primary> ();
	}

protected:
	ImplementationLocalPOA ()
	{}
};

}
}

#endif
