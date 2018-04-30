// Standard Nirvana interface implementation

#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "AbstractBase.h"

namespace PortableServer {
namespace Nirvana {

template <class S, class I> class Servant;

template <class S, class I>
class InterfaceImpl :
	public Bridge <I>,
	public Skeleton <S, I>
{
public:
	T_ptr <I> _this ()
	{
		return this;
	}

protected:
	InterfaceImpl () :
		Bridge <I> (sm_epv)
	{}
};

template <class S>
class AbstractBaseImpl :
	public InterfaceImpl <S, AbstractBase>,
	public RefCountBase
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::sm_epv.interface);
		return static_cast <S&> (*bridge);
	}

	template <class I>
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf, Environment* env)
	{
		try {
			S::_implementation (static_cast <Bridge <I>*> (itf))._add_ref ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return itf;
	}

	template <class I>
	static void _release (Bridge <Interface>* itf)
	{
		try {
			S::_implementation (static_cast <Bridge <I>*> (itf))._remove_ref ();
		} catch (...) {
		}
	}

	template <class I>
	static Bridge <I>& _narrow (Bridge <AbstractBase>& base)
	{
		return static_cast <Bridge <I>&> (static_cast <S&> (base));
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, Environment* env)
	{
		try {
			return static_cast <Bridge <Base>*> (&S::_implementation (derived));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S, class Primary, class ... Base> // Base includes all derived interfaces, directly and indirectly
class Implementation :
	public AbstractBaseImpl <S>,
	public InterfaceImpl <S, Base> ...,
	public InterfaceImpl <S, Primary>
{
public:
	typedef Primary _PrimaryInterface;

	static const Char* _primary_interface ()
	{
		return Bridge <Primary>::_primary_interface ();
	}

	static ::CORBA::Nirvana::Bridge <::CORBA::Nirvana::Interface>* _find_interface (Bridge <AbstractBase>& base, const Char* id)
	{
		return InterfaceImpl <S, Primary>::_find_interface (base, id);
	}

	static Boolean ___is_a (const Char* type_id)
	{
		return Bridge <Primary>::___is_a (type_id);
	}

	T_ptr <Primary> _this ()
	{
		return InterfaceImpl <S, Primary>::_this ();
	}
};

}
}

#endif
