#ifndef NIRVANA_ORB_ABSTRACTBASEPOA_H_
#define NIRVANA_ORB_ABSTRACTBASEPOA_H_

#include "AbstractBase_s.h"
#include "ReferenceCounterPOA.h"

namespace CORBA {
namespace Nirvana {

//! POA implementation of AbstractBase

template <>
class NIRVANA_NOVTABLE ServantPOA <AbstractBase> :
	public ServantPOA <ReferenceCounter>,
	public InterfaceImplBase <ServantPOA <AbstractBase>, AbstractBase>
{
public:
	virtual Interface_ptr _query_interface (const String& id) = 0;

protected:
	ServantPOA ()
	{}
};

}
}

#endif
