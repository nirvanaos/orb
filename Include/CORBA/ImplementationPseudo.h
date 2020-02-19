// The Nirvana project.
// Object Request Broker.
// Pseudo object servant implementation.
// This header is intended for core and proxy code.
#ifndef NIRVANA_ORB_IMPLEMENTATIONPSEUDO_H_
#define NIRVANA_ORB_IMPLEMENTATIONPSEUDO_H_

#include <Nirvana/Nirvana.h>
#include "ServantStatic.h"

namespace CORBA {
namespace Nirvana {

//! \class ImplementationPseudo
//!
//! \brief An implementation of a pseudo object (like proxy).
//!
//! You also have to derive your servant from some life cycle implementation
//! such as LifeCycleRefCntLink.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationPseudo :
	public ServantTraits <S>,
	public InterfaceImplBase <S, Bases> ...,
	public InterfaceImplBase <S, Primary>
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
	ImplementationPseudo ()
	{}
};

//! \class ImplementationStaticPseudo
//!
//! \brief Static implementation of a pseudo interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationStaticPseudo :
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
