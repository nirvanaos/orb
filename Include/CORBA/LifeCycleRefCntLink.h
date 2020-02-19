// The Nirvana project.
// Object Request Broker.
// This header is intended exclusively for proxy code.
#ifndef NIRVANA_ORB_LIFECYCLEREFCNTLINK_H_
#define NIRVANA_ORB_LIFECYCLEREFCNTLINK_H_

#include "DynamicServant_s.h"
#include "ReferenceCounter_s.h"

namespace CORBA {
namespace Nirvana {

//! \class ReferenceCounterLink
//!
//! \brief Delegates reference counting operations to
//! core implementation of the ReferenceCounter interface.
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
	ReferenceCounterLink () :
		reference_counter_ (ReferenceCounter::_nil ())
	{}

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

//! Life cycle implementation based on core ReferenceCounter
template <class S>
class LifeCycleRefCntLink :
	public InterfaceImpl <S, DynamicServant>,
	public LifeCycleRefCnt <S>,
	public ReferenceCounterLink
{
protected:
	LifeCycleRefCntLink () :
		ReferenceCounterLink (this)
	{}
};

}
}

#endif
