#ifndef NIRVANA_ORB_REFERENCECOUNTERPOA_H_
#define NIRVANA_ORB_REFERENCECOUNTERPOA_H_

#include "DynamicServantPOA.h"
#include "ReferenceCounter_s.h"

namespace CORBA {
namespace Nirvana {

template <>
class NIRVANA_NOVTABLE ServantPOA <ReferenceCounter> :
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
