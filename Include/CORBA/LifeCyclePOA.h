// The Nirvana project.
// Object Request Broker.

#ifndef NIRVANA_ORB_LIFECYCLEPOA_H_
#define NIRVANA_ORB_LIFECYCLEPOA_H_

#include "ReferenceCounterLink.h"
#include "ReferenceCounter_s.h"
#include "ServantPOA.h"

namespace CORBA {
namespace Nirvana {

template <class I> class ServantPOA;

template <>
class ServantPOA <DynamicServant> :
	public ServantTraitsPOA,
	public DynamicServantImpl <ServantPOA <DynamicServant> >
{
public:
	virtual ~ServantPOA ()
	{}

	virtual void _add_ref ()
	{
		DynamicServantImpl <ServantPOA <DynamicServant> >::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		DynamicServantImpl <ServantPOA <DynamicServant> >::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return DynamicServantImpl <ServantPOA <DynamicServant> >::_refcount_value ();
	}

protected:
	ServantPOA ()
	{}
};

template <>
class ServantPOA <ReferenceCounter> :
	public ServantPOA <DynamicServant>,
	public InterfaceImpl <ServantPOA <ReferenceCounter>, ReferenceCounter>
{
protected:
	ServantPOA ()
	{}
};

typedef ServantPOA <ReferenceCounter> LifeCyclePOA;

}
}

#endif
