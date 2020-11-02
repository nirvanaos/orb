#ifndef NIRVANA_ORB_IMPLEMENTATIONLOCALPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONLOCALPOA_H_

#include "LocalObjectPOA.h"
#include "FindInterface.h"

namespace CORBA {
namespace Nirvana {

//! \class ImplementationLocalPOA
//!
//! \brief Portable implementation of localinterface.
//!
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly.

template <class Primary, class ... Bases>
class NIRVANA_NOVTABLE ImplementationLocalPOA :
	public virtual ServantPOA <LocalObject>,
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
	ImplementationLocalPOA ()
	{}
};

}
}

#endif
