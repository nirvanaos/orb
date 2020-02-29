#ifndef NIRVANA_ORB_LOCALOBJECTIMPL_H_
#define NIRVANA_ORB_LOCALOBJECTIMPL_H_

#include "AbstractBaseImpl.h"
#include "Object_s.h"
#include "LocalObject_s.h"
#include "ObjectLink.h"

namespace CORBA {
namespace Nirvana {

//! \brief Implementation of CORBA::LocalObject
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, LocalObject> :
	public InterfaceImpl <S, AbstractBase>,
	public InterfaceImplBase <S, Object>,
	public InterfaceImplBase <S, LocalObject>,
	public ObjectLink
{
protected:
	InterfaceImpl ()
	{}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}
};

}
}

#endif
