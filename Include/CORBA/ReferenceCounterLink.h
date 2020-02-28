// The Nirvana project.
// Object Request Broker.
#ifndef NIRVANA_ORB_REFERENCECOUNTERLINK_H_
#define NIRVANA_ORB_REFERENCECOUNTERLINK_H_

#include "ReferenceCounter.h"
#include "DynamicServant_s.h"
#include "ServantImpl.h"

namespace CORBA {
namespace Nirvana {

/// \class ReferenceCounterLink
///
/// \brief Delegates reference counting to core
///        implementation of the ReferenceCounter interface.
class ReferenceCounterLink
{
	ReferenceCounterLink (const ReferenceCounterLink&) = delete;
public:
	void _add_ref ()
	{
		reference_counter_->_add_ref ();
	}

	void _remove_ref ()
	{
		reference_counter_->_remove_ref ();
	}

	ULong _refcount_value ()
	{
		return reference_counter_->_refcount_value ();
	}

protected:
	ReferenceCounterLink (Bridge <DynamicServant>* dynamic);

	ReferenceCounterLink& operator = (const ReferenceCounterLink&)
	{
		return *this; // Do nothing
	}

	~ReferenceCounterLink ();

protected:
	ReferenceCounter_ptr _reference_counter () const
	{
		return reference_counter_;
	}

private:
	ReferenceCounter_ptr reference_counter_;
};

/// \brief Dynamic servant implementation
/// \tparam S Servant class
///
/// We don't implement reference counting in the object implementation
/// because implementation of the stateless objects resides in the
/// read-only memory and can't be changed.
/// So reference counting for objects implemented at the core.
template <class S>
class DynamicServantImpl :
	public InterfaceImpl <S, DynamicServant>,
	public ReferenceCounterLink,
	public LifeCycleRefCnt <S>
{
protected:
	DynamicServantImpl () :
		ReferenceCounterLink (this)
	{}
};

}
}

#endif
