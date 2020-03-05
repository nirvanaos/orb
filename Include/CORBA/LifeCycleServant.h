// The Nirvana project.
// Object Request Broker.
#ifndef NIRVANA_ORB_LIFECYCLESERVANT_H_
#define NIRVANA_ORB_LIFECYCLESERVANT_H_

#include "ReferenceCounter.h"
#include "DynamicServant_s.h"
#include "ServantImpl.h"
#include "core_objects.h"

namespace CORBA {
namespace Nirvana {

/// \class ReferenceCounterLink
///
/// \brief Delegates reference counting to core
///        implementation of the ReferenceCounter interface.
class ReferenceCounterLink :
	public Bridge <DynamicServant>
{
	ReferenceCounterLink (const ReferenceCounterLink&) = delete;
public:
#ifdef NIRVANA_C14

	void* operator new (size_t size)
	{
		return g_object_factory->memory_allocate (size);
	}

	void operator delete (void* p, size_t size)
	{
		g_object_factory->memory_release (p, size);
	}

#else

	void* operator new (size_t size)
	{
		size += sizeof (size_t);
		size_t* hdr = (size_t*)g_object_factory->memory_allocate (size);
		*hdr = size;
		return hdr + 1;
	}

	void operator delete (void* p, size_t size)
	{
		size_t* hdr = (size_t*)p - 1;
		g_object_factory->memory_release (hdr, *hdr);
	}

#endif

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
	ReferenceCounterLink (const Bridge <DynamicServant>::EPV& epv);

	ReferenceCounterLink& operator = (const ReferenceCounterLink&)
	{
		return *this; // Do nothing
	}

protected:
	ReferenceCounter_ptr _reference_counter () const
	{
		return reference_counter_;
	}

private:
	ReferenceCounter_var reference_counter_;
};

/// \brief Dynamic servant life cycle implementation
/// \tparam S Servant class
///
/// We don't implement reference counting in the object implementation
/// because implementation of the stateless objects resides in the
/// read-only memory and can't be changed.
/// So reference counting for objects implemented at the core.
template <class S>
class LifeCycleServant :
	public Skeleton <S, DynamicServant>,
	public ReferenceCounterLink,
	public LifeCycleRefCnt <S>
{
protected:
	LifeCycleServant () :
		ReferenceCounterLink (Skeleton <S, DynamicServant>::epv_)
	{}
};

}
}

#endif