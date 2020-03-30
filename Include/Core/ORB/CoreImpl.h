/// \file CoreImpl.h
#ifndef NIRVANA_ORB_CORE_COREIMPL_H_
#define NIRVANA_ORB_CORE_COREIMPL_H_

#include <CORBA/Server.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

/// \brief Core implementation of ServantBase and LocalObject.
template <class T, class I, class Proxy>
class CoreImpl :
	public Proxy,
	public LifeCycleNoCopy <T>,
	public ServantTraits <T>,
	public InterfaceImplBase <T, I>
{
public:
	using ServantTraits <T>::_implementation;
	using LifeCycleNoCopy <T>::__duplicate;
	using LifeCycleNoCopy <T>::__release;

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, String_in id, EnvironmentBridge* env)
	{
		return ServantTraits <T>::template _wide <Base, Derived> (derived, id, env);
	}

	template <>
	static Bridge <ReferenceCounter>* _wide <ReferenceCounter, I> (Bridge <I>* derived, String_in id, EnvironmentBridge* env)
	{
		return nullptr; // ReferenceCounter base is not implemented, return nullptr.
	}
};

}
}
}

#endif
