#ifndef NIRVANA_ORB_IMPLEMENTATIONLOCALSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONLOCALSTATIC_H_

#include "ObjectStatic.h"

namespace CORBA {
namespace Nirvana {

//! \class ImplementationStatic
//!
//! \brief Static implementation of a local interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationStatic :
	public InterfaceStatic <S, Object>,
	public InterfaceStatic <S, Bases>...,
	public InterfaceStatic <S, Primary>
{
public:
	Interface_ptr _query_interface (const String& id)
	{
		return FindInterface <Primary, Bases..., Object>::find (*(S*)0, id);
	}

	static I_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (InterfaceStatic <S, Object>::_get_proxy ());
	}
};

}
}

#endif
