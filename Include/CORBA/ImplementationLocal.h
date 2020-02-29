#ifndef NIRVANA_ORB_IMPLEMENTATIONLOCAL_H_
#define NIRVANA_ORB_IMPLEMENTATIONLOCAL_H_

#include "LocalObjectImpl.h"
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
//!    Don't include AbstractBase in base list.

template <class S, class Primary, class ... Bases>
class ImplementationLocal :
	public InterfaceImpl <S, LocalObject>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>
{
public:
	Interface_ptr _query_interface (const String& id)
	{
		return FindInterface <Primary, Bases..., CORBA::Object>::find (static_cast <S&> (*this), id);
	}

	I_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (ObjectLink::_get_proxy ());
	}

protected:
	ImplementationLocal ()
	{}
};

}
}

#endif
