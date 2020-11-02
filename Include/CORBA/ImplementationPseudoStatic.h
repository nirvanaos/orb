// The Nirvana project.
// Object Request Broker.
// Pseudo object static servant implementation.
// This header is intended for core and proxy code.
#ifndef NIRVANA_ORB_IMPLEMENTATIONPSEUDOSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONPSEUDOSTATIC_H_

#include "ServantStatic.h"
#include "LifeCycleStatic.h"

namespace CORBA {
namespace Nirvana {

//! \class ImplementationPseudoStatic
//!
//! \brief Static implementation of a pseudo interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationPseudoStatic :
	public ServantTraitsStatic <S>,
	public LifeCycleStatic,
	public InterfaceStaticBase <S, Bases> ...,
	public InterfaceStaticBase <S, Primary>
{
public:
	static Primary* _get_ptr ()
	{
		return static_cast <Primary*> (InterfaceStaticBase <S, Primary>::_bridge ());
	}
};

}
}

#endif
