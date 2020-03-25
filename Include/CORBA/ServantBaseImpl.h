#ifndef NIRVANA_ORB_SERVANTBASEIMPL_H_
#define NIRVANA_ORB_SERVANTBASEIMPL_H_

#include "AbstractBaseImpl.h"
#include "ServantBaseLink.h"
#include "ServantBase_s.h"

namespace CORBA {
namespace Nirvana {

//! Implementation of PortableServer::ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, PortableServer::ServantBase> :
	public InterfaceImpl <S, AbstractBase>,
	public Skeleton <S, PortableServer::ServantBase>,
	public ServantBaseLink
{
protected:
	InterfaceImpl () :
		ServantBaseLink (Skeleton <S, PortableServer::ServantBase>::epv_)
	{
		_construct ();
	}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}
};

}
}

#endif
