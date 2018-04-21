// Static interface implementation

#ifndef NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_
#define NIRVANA_ORB_IMPLEMENTATIONSTATIC_H_

#include "AbstractBase.h"

namespace PortableServer {
namespace Nirvana {

template <class S, class I> class ServantStatic;

template <class S, class I>
class InterfaceStatic :
	public Skeleton <S, I>
{
	InterfaceStatic ();	// Never be instantiated
public:
	static T_ptr <I> _this ()
	{
		return (Bridge <I>*)&sm_bridge;
	}

private:
	static const typename Bridge <I>::EPV& sm_bridge;
};

template <class S, class I>
const typename Bridge <I>::EPV& InterfaceStatic<S, I>::sm_bridge = sm_epv;

class LifeCycleStatic
{
public:
	template <class I>
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf, Environment*)
	{
		return itf;
	}

	template <class I>
	static void _release (Bridge <Interface>*)
	{}
};

template <class S>
class AbstractBaseStatic :
	public InterfaceStatic <S, AbstractBase>,
	public LifeCycleStatic
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		return (S*)0;
	}

	template <class I, class B>
	static Bridge <I>& _narrow (B&)
	{
		return InterfaceStatic <S, I>::_this ();
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>*, Environment*)
	{
		return InterfaceStatic <S, Base>::_this ();
	}
};

}
}

#endif
