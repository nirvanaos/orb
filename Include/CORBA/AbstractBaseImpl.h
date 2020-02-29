#ifndef NIRVANA_ORB_ABSTRACTBASEIMPL_H_
#define NIRVANA_ORB_ABSTRACTBASEIMPL_H_

#include "AbstractBase_s.h"
#include "DynamicServantImpl.h"

namespace CORBA {
namespace Nirvana {

template <class S>
class InterfaceImpl <S, AbstractBase> :
	public InterfaceImpl <S, DynamicServant>,
	public InterfaceImplBase <S, AbstractBase>
{};

}
}

#endif
