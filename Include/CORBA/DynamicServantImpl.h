// The Nirvana project.
// Object Request Broker.

#ifndef NIRVANA_ORB_DYNAMICSERVANTIMPL_H_
#define NIRVANA_ORB_DYNAMICSERVANTIMPL_H_

#include "ReferenceCounterLink.h"

namespace CORBA {
namespace Nirvana {

/// \brief Reference counting implementation for servant
template <class S>
class InterfaceImpl <S, DynamicServant> :
	public ServantTraits <S>,
	public LifeCycleServant <S>
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
		return *(this->_reference_counter ());
	}
};

}
}

#endif
