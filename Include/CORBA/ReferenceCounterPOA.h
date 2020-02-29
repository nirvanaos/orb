#ifndef NIRVANA_ORB_REFERENCECOUNTERPOA_H_
#define NIRVANA_ORB_REFERENCECOUNTERPOA_H_

#include "DynamicServantPOA.h"
#include "ReferenceCounter_s.h"

namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA <ReferenceCounter> :
	public ServantPOA <DynamicServant>,
	public InterfaceImpl <ServantPOA <ReferenceCounter>, ReferenceCounter>
{
protected:
	ServantPOA ()
	{}
};

}
}

#endif
