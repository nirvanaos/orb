#ifndef NIRVANA_ORB_ABSTRACTBASESTATIC_H_
#define NIRVANA_ORB_ABSTRACTBASESTATIC_H_

#include "AbstractBase_s.h"
#include "LifeCycleStatic.h"
#include "ServantStatic.h"

namespace CORBA {
namespace Nirvana {

//! Static implementation of AbstractBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceStatic <S, AbstractBase> :
	public InterfaceStaticBase <S, AbstractBase>,
	public ServantTraitsStatic <S>,
	public LifeCycleStatic
{};

}
}

#endif
