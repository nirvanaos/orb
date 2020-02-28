// The Nirvana project.
// Object Request Broker.

#ifndef NIRVANA_ORB_LIFECYCLESERVANT_H_
#define NIRVANA_ORB_LIFECYCLESERVANT_H_

#include "ReferenceCounterLink.h"
#include "DynamicServant_s.h"
#include "ServantImpl.h"

namespace CORBA {
namespace Nirvana {

/// \class LifeCycleServant
/// \tparam S Servant class
/// \brief Dynamic servant implementation
///
/// We don't implement reference counting in the object implementation
/// because implementation of the stateless objects resides in the
/// read-only memory and can't be changed.
/// So reference counting for objects implemented at the core.
template <class S>
class LifeCycleServant :
	public InterfaceImpl <S, DynamicServant>,
	public ReferenceCounterLink,
	public LifeCycleRefCnt <S>
{
public:
	/// \brief For the performance reasons, we don't implement 
	///        the ReferenceCounter interface and just return bridge to 
	///        the core implementation.
	///
	/// If you need to override `_add_ref()` and `_remove_ref()` operations
	/// for debugging purposes, you can add InterfaceImpl <S, ReferenceCounter>
	/// as class base and override operator Bridge <ReferenceCounter>&.
	operator Bridge <ReferenceCounter>& ()
	{
		return *reference_counter_;
	}

protected:
	LifeCycleServant (ReferenceCounter_ptr rc) :
		ReferenceCounterLink (rc)
	{}

	/// Constructor without parameters intended for pseudo objects
	LifeCycleServant () :
		ReferenceCounterLink (this)
	{}
};

}
}

#endif
