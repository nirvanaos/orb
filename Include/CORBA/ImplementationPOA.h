/// \file ImplementationPOA.h
/// POA (virtual) interface implementation.

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
#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "ServantBasePOA.h"
#include "FindInterface.h"

namespace CORBA {
namespace Nirvana {

//! \class ImplementationPOA
//!
//! \brief Portable implementation of interface.
//!
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class Primary, class ... Bases>
class ImplementationPOA :
	public virtual ServantPOA <PortableServer::ServantBase>,
	public virtual ServantPOA <Bases>...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	virtual Interface_ptr _query_interface (const String& id)
	{
#ifdef _DEBUG
		Bridge <AbstractBase>* ab = this;
		const Bridge <AbstractBase>::EPV& epv = ab->_epv ();
		assert (!strcmp (epv.header.interface_id, Bridge <AbstractBase>::repository_id_));
#endif
		return FindInterface <Primary, Bases...>::find (static_cast <ServantPOA <Primary>&> (*this), id);
	}

	I_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (this->_get_proxy ());
	}

protected:
	ImplementationPOA ()
	{}
};

}
}

#endif
