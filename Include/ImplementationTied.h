// Tied interface implementation

#ifndef NIRVANA_ORB_IMPLEMENTATIONTIED_H_
#define NIRVANA_ORB_IMPLEMENTATIONTIED_H_

#include "AbstractBase.h"

namespace PortableServer {
namespace Nirvana {

// Forward declarations

template <class S> class AbstractBaseTied;
template <class I> class InterfaceTied;
template <class S, class I> class ServantTied;

// Abstract base implementation

template <>
class InterfaceTied <AbstractBase> :
	public Bridge <AbstractBase>,
	public RefCountBase
{
public:
	template <class I>
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf, Environment* env);

	template <>
	static Bridge <Interface>* _duplicate <AbstractBase> (Bridge <Interface>* itf, Environment* env);

	template <class I>
	static void _release (Bridge <Interface>* itf);

	template <>
	static void _release <AbstractBase> (Bridge <Interface>* itf);

protected:
	template <class S>
	friend class AbstractBaseTied;

	InterfaceTied (const EPV& epv, void* implementation) :
		Bridge <AbstractBase> (epv),
		m_implementation (implementation)
	{}

	InterfaceTied ()
	{
#ifdef _DEBUG
		assert (false);
		m_implementation = 0;
#endif
	}

	void* _implementation () const
	{
		assert (m_implementation);
		return m_implementation;
	}

	AbstractBase_ptr _this ()
	{
		return this;
	}

private:
	static InterfaceTied <AbstractBase>& _get_base (Bridge <Interface>* itf);
	static InterfaceTied <AbstractBase>& _get_base (Bridge <AbstractBase>* itf);

private:
	void* m_implementation;
};

template <class I>
class InterfaceTiedBase :
	public Bridge <I>,
	public virtual InterfaceTied <AbstractBase>
{
protected:
	InterfaceTiedBase (const typename Bridge <I>::EPV& epv) :
		Bridge <I> (epv)
	{}

	InterfaceTiedBase ()
	{}

	T_ptr <I> _this ()
	{
		return static_cast <Bridge <I>*> (this);
	}
};

template <class I>
Bridge <Interface>* InterfaceTied <AbstractBase>::_duplicate (Bridge <Interface>* itf, Environment* env)
{
	try {
		_get_base (itf)._add_ref ();
	} catch (const Exception& e) {
		env->set_exception (e);
	} catch (...) {
		env->set_unknown_exception ();
	}
	return itf;
}

template <>
Bridge <Interface>* InterfaceTied <AbstractBase>::_duplicate <AbstractBase> (Bridge <Interface>* itf, Environment* env)
{
	try {
		_get_base (static_cast <Bridge <AbstractBase>*> (itf))._add_ref ();
	} catch (const Exception& e) {
		env->set_exception (e);
	} catch (...) {
		env->set_unknown_exception ();
	}
	return itf;
}

template <class I>
void InterfaceTied <AbstractBase>::_release (Bridge <Interface>* itf)
{
	try {
		_get_base (itf)._remove_ref ();
	} catch (...) {
	}
}

template <>
void InterfaceTied <AbstractBase>::_release <AbstractBase> (Bridge <Interface>* itf)
{
	try {
		_get_base (static_cast <Bridge <AbstractBase>*> (itf))._remove_ref ();
	} catch (...) {
	}
}

template <class S>
class AbstractBaseTied :
	public virtual InterfaceTied <AbstractBase>,
	public Skeleton <S, AbstractBase>
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::sm_epv.interface);
		return *reinterpret_cast <S*> (static_cast <InterfaceTied <I>&> (*bridge)._implementation ());
	}

	static S& _implementation (Bridge <AbstractBase>& base)
	{
		return *reinterpret_cast <S*> (static_cast <InterfaceTied <AbstractBase>&> (base)._implementation ());
	}

	template <class I>
	static Bridge <I>& _narrow (Bridge <AbstractBase>& base)
	{
		static_cast <Bridge <I>&> (_implementation (base));
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* itf, Environment* env)
	{
		try {
			_check_pointer (itf, Skeleton <S, Derived>::sm_epv.interface);
			return static_cast <Bridge <Base>*> (static_cast <InterfaceTied <Derived>*> (itf));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

protected:
	AbstractBaseTied (S& implementation) :
		InterfaceTied <AbstractBase> (sm_epv, &implementation)
	{}
};

}
}

#endif
