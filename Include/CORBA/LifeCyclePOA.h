// The Nirvana project.
// Object Request Broker.

#ifndef NIRVANA_ORB_LIFECYCLEPOA_H_
#define NIRVANA_ORB_LIFECYCLEPOA_H_

#include "ReferenceCounterLink.h"
#include "DynamicServant_s.h"
#include "ReferenceCounter_s.h"
#include "ServantPOA.h"

namespace CORBA {
namespace Nirvana {

template <class I> class ServantPOA;

template <>
class ServantPOA <DynamicServant> :
	public ServantTraitsPOA,
	public InterfaceImpl <ServantPOA <DynamicServant>, DynamicServant>,
	public ReferenceCounterLink,
	public LifeCycleRefCnt <ServantPOA <DynamicServant> >
{
public:
	virtual ~ServantPOA ()
	{}

	virtual void _add_ref ()
	{
		ReferenceCounterLink::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		ReferenceCounterLink::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return ReferenceCounterLink::_refcount_value ();
	}

protected:
	ServantPOA (ReferenceCounter_ptr rc) :
		ReferenceCounterLink (rc)
	{}

	/// Constructor without parameters intended for pseudo objects
	ServantPOA () :
		ReferenceCounterLink (this)
	{}
};

template <>
class ServantPOA <ReferenceCounter> :
	public ServantPOA <DynamicServant>,
	public InterfaceImpl <ServantPOA <ReferenceCounter>, ReferenceCounter>
{
protected:
	ServantPOA (ReferenceCounter_ptr rc) :
		ServantPOA <DynamicServant> (rc)
	{}

	/// Constructor without parameters intended for pseudo objects
	ServantPOA ()
	{}
};

typedef ServantPOA <ReferenceCounter> LifeCyclePOA;

}
}

#endif
