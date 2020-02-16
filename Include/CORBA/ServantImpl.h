#ifndef NIRVANA_ORB_SERVANTIMPL_H_
#define NIRVANA_ORB_SERVANTIMPL_H_

#include "Environment_c.h"
#include "RepositoryId.h"
#include "I_ptr.h"
#include "LifeCycle.h"

namespace CORBA {
namespace Nirvana {

extern void _check_pointer (const Interface* obj, const Interface::EPV& epv);

template <class S, class I> class Skeleton;

//! Standard servant mix-in.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Primary interface.
template <class S, class I> class Servant;

//! Standard (dynamic) servant traits.
//! \tparam S Servant class, derived from this.
template <class S>
class ServantTraits
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::epv_.interface);
		return static_cast <S&> (*bridge);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env)
	{
		try {
			if (!RepositoryId::compatible (Bridge <Base>::interface_id_, id))
				::Nirvana::throw_MARSHAL ();
			return &static_cast <Bridge <Base>&> (S::_implementation (derived));
		} catch (const Exception& e) {
			set_exception (env, e);
		} catch (...) {
			set_unknown_exception (env);
		}
		return nullptr;
	}
};

//! Standard interface implementation.
//! \tparam S Servant class implementing operations. Must derive from this mix-in.
//! \tparam I Interface.
template <class S, class I>
class InterfaceImplBase :
	public Bridge <I>,
	public Skeleton <S, I>
{
protected:
	InterfaceImplBase () :
		Bridge <I> (Skeleton <S, I>::epv_)
	{}
};

template <class S, class I>
class InterfaceImpl :
	public InterfaceImplBase <S, I>
{};

}
}

#endif
