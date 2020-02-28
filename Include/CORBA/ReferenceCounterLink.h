// The Nirvana project.
// Object Request Broker.
#ifndef NIRVANA_ORB_REFERENCECOUNTERLINK_H_
#define NIRVANA_ORB_REFERENCECOUNTERLINK_H_

#include "ReferenceCounter.h"
#include "DynamicServant.h"

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

	ReferenceCounterLink (ReferenceCounter_ptr rc) :
		reference_counter_ (rc)
	{}

	ReferenceCounterLink& operator = (const ReferenceCounterLink&)
	{
		return *this; // Do nothing
	}

	~ReferenceCounterLink ();

protected:
	ReferenceCounter_ptr reference_counter_;
};

}
}

#endif
