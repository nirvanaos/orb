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
		return _bridge ();
	}

	static Bridge <I>* _bridge ()
	{
		return (Bridge <I>*)&sm_bridge;
	}

private:
	static const typename Bridge <I>::EPV* sm_bridge;
};

template <class S, class I>
const typename Bridge <I>::EPV* InterfaceStatic<S, I>::sm_bridge = &sm_epv;

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
		return *(S*)0;
	}

	template <class I, class B>
	static Bridge <I>& _narrow (B&)
	{
		return *InterfaceStatic <S, I>::_bridge ();
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>*, Environment*)
	{
		return InterfaceStatic <S, Base>::_bridge ();
	}
};

template <class S, class Primary, class ... Base> // Base includes all derived interfaces, directly and indirectly
class ImplementationStatic :
	public AbstractBaseStatic <S>,
	public InterfaceStatic <S, Base> ...,
	public InterfaceStatic <S, Primary>
{
public:
	typedef Primary _PrimaryInterface;

	static const Char* _primary_interface ()
	{
		return Bridge <Primary>::_primary_interface ();
	}

	static ::CORBA::Nirvana::Bridge <::CORBA::Nirvana::Interface>* _find_interface (Bridge <AbstractBase>& base, const Char* id)
	{
		return InterfaceStatic <S, Primary>::_find_interface (base, id);
	}

	static Boolean ___is_a (const Char* type_id)
	{
		return Bridge <Primary>::___is_a (type_id);
	}

	static T_ptr <Primary> _this ()
	{
		return InterfaceStatic <S, Primary>::_this ();
	}
};

}
}

#endif
