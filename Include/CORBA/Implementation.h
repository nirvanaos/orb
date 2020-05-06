// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ServantBaseImpl.h"
#include "FindInterface.h"

namespace CORBA {
namespace Nirvana {

//! \class Implementation
//!
//! \brief An implementation of interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class Implementation :
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>,
	public InterfaceImpl <S, PortableServer::ServantBase> // ServantBase implementation must be constructed after all interfaces.
{
public:
	Interface_ptr _query_interface (const String& id)
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}

	I_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (ServantBaseLink::_get_proxy ());
	}

protected:
	Implementation ()
	{}
};

}
}

#endif
