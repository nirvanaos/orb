// The Nirvana project.
// Object Request Broker.

#ifndef NIRVANA_ORB_LIFECYCLEPOA_H_
#define NIRVANA_ORB_LIFECYCLEPOA_H_

#include "ReferenceCounterLink.h"
#include "ServantPOA.h"

namespace CORBA {
namespace Nirvana {

template <>
class ServantPOA <DynamicServant> :
	public ServantTraitsPOA,
	public LifeCycleServant <ServantPOA <DynamicServant> >
{
public:
	virtual ~ServantPOA ()
	{}

	virtual void _add_ref ()
	{
		LifeCycleServant <ServantPOA <DynamicServant> >::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		LifeCycleServant <ServantPOA <DynamicServant> >::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return LifeCycleServant <ServantPOA <DynamicServant> >::_refcount_value ();
	}

protected:
	ServantPOA ()
	{}
};

}
}

#endif
