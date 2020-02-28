// The Nirvana project.
// Object Request Broker.
// Pseudo object servant virtual implementation.
// This header is intended for core and proxy code.
#ifndef NIRVANA_ORB_IMPLEMENTATIONPSEUDOPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPSEUDOPOA_H_

#include "ServantImpl.h"
#include "I_ptr.h"
#include "LifeCyclePOA.h"

namespace CORBA {
namespace Nirvana {

//! \class ImplementationPseudoPOA
//!
//! \brief POA-style (virtual) implementation of the pseudo interface.
//!
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class Primary, class ... Bases>
class ImplementationPseudoPOA :
	public virtual LifeCyclePOA,
	public virtual ServantPOA <Bases>...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	//! \fn I_ptr <Primary> _get_ptr ()
	//!
	//! \brief Gets the pointer.
	//!   Works like _this() method but doesn't increment the reference counter.
	//!
	//! \return The primary interface pointer.

	I_ptr <Primary> _get_ptr ()
	{
		return I_ptr <Primary> (&static_cast <Primary&> (static_cast <Bridge <Primary>&> (*this)));
	}

protected:
	ImplementationPseudoPOA ()
	{}
};

}
}

#endif
