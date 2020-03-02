// Nirvana project
// Object Request Broker
// POA (virtual) interface implementation.
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
		assert (!strcmp (epv.header.interface_id, Bridge <AbstractBase>::interface_id_));
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
