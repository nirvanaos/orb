#ifndef NIRVANA_ORB_LOCALOBJECTIMPL_H_
#define NIRVANA_ORB_LOCALOBJECTIMPL_H_

#include "AbstractBaseImpl.h"
#include "LocalObjectLink.h"
#include "LocalObject_s.h"

namespace CORBA {
namespace Nirvana {

//! \brief Implementation of CORBA::LocalObject
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, LocalObject> :
	public InterfaceImpl <S, AbstractBase>,
	public Skeleton <S, LocalObject>,
	public LocalObjectLink
{
protected:
	InterfaceImpl () :
		LocalObjectLink (Skeleton <S, LocalObject>::epv_, *this)
	{}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}
};

}
}

#endif
