// Nirvana project
// Object Request Broker
// Tied interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONTIED_H_
#define NIRVANA_ORB_IMPLEMENTATIONTIED_H_

#include "AbstractBase.h"

namespace CORBA {
namespace Nirvana {

// Forward declarations

template <class I> class InterfaceTiedBase;
template <class S, class I> class InterfaceTied;
template <class T, class I> class ServantTied;
template <class T, class S> class AbstractBaseTied;

// Abstract base implementation

template <>
class InterfaceTiedBase <AbstractBase> :
	public Bridge <AbstractBase>,
	public RefCountBase
{
public:
	template <class I>
	static Bridge <Interface>* _duplicate (Bridge <Interface>* itf, EnvironmentBridge* env);

	template <>
	static Bridge <Interface>* _duplicate <AbstractBase> (Bridge <Interface>* itf, EnvironmentBridge* env);

	template <class I>
	static void _release (Bridge <Interface>* itf);

	template <>
	static void _release <AbstractBase> (Bridge <Interface>* itf);

	Boolean _is_owner () const
	{
		return m_release;
	}

	void _is_owner (Boolean rel)
	{
		m_release = rel;
	}

protected:
	InterfaceTiedBase ()
	{}

	AbstractBase_ptr _this ()
	{
		return this;
	}

protected:
	template <class T, class S>
	friend class AbstractBaseTied;

	virtual Bridge <Interface>* _find_interface (const Char* id) = 0;

	void* _implementation () const
	{
		assert (m_implementation);
		return m_implementation;
	}

	void* _tied_object () const
	{
		return m_implementation;
	}

	void _tied_object (void* obj, Boolean release)
	{
		m_implementation = obj;
		m_release = release;
	}

private:
	static InterfaceTiedBase <AbstractBase>& _get_base (Bridge <Interface>* itf);
	static InterfaceTiedBase <AbstractBase>& _get_base (Bridge <AbstractBase>* itf);

private:
	void* m_implementation;
	Boolean m_release;
};

template <class I>
Bridge <Interface>* InterfaceTiedBase <AbstractBase>::_duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
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
Bridge <Interface>* InterfaceTiedBase <AbstractBase>::_duplicate <AbstractBase> (Bridge <Interface>* itf, EnvironmentBridge* env)
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
void InterfaceTiedBase <AbstractBase>::_release (Bridge <Interface>* itf)
{
	try {
		_get_base (itf)._remove_ref ();
	} catch (...) {
	}
}

template <>
void InterfaceTiedBase <AbstractBase>::_release <AbstractBase> (Bridge <Interface>* itf)
{
	try {
		_get_base (static_cast <Bridge <AbstractBase>*> (itf))._remove_ref ();
	} catch (...) {
	}
}

template <class I>
class InterfaceTiedBase :
	public Bridge <I>,
	public virtual InterfaceTiedBase <AbstractBase>
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

template <class S, class I>
class InterfaceTied :
	public InterfaceTiedBase <I>,
	public Skeleton <S, I>
{
protected:
	InterfaceTied () :
		InterfaceTiedBase <I> (Skeleton <S, I>::sm_epv)
	{}
};

template <class T, class S>
class AbstractBaseTied :
	public virtual InterfaceTiedBase <AbstractBase>,
	public Skeleton <S, AbstractBase>
{
public:
	template <class I>
	static T& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::sm_epv.interface);
		return *reinterpret_cast <T*> (static_cast <InterfaceTiedBase <I>&> (*bridge)._implementation ());
	}

	static T& _implementation (Bridge <AbstractBase>* bridge)
	{
		_check_pointer (bridge, sm_epv.interface);
		return *reinterpret_cast <S*> (static_cast <InterfaceTiedBase <AbstractBase>&> (*bridge)._implementation ());
	}

	template <class I>
	static Bridge <I>& _narrow (S& servant)
	{
		static_cast <Bridge <I>&> (servant);
	}

	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* itf, EnvironmentBridge* env)
	{
		try {
			_check_pointer (itf, Skeleton <S, Derived>::sm_epv.interface);
			return static_cast <Bridge <Base>*> (static_cast <S*> (itf));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, EnvironmentBridge* env)
	{
		try {
			_check_pointer (base, Skeleton <S, AbstractBase>::sm_epv.interface);
			_check_pointer (id);
			return static_cast <InterfaceTiedBase <AbstractBase>&> (*base)._find_interface (id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	T* _tied_object () const
	{
		return reinterpret_cast <T*> (InterfaceTiedBase <AbstractBase>::_tied_object ());
	}

	void _tied_object (T* obj, Boolean release = TRUE)
	{
		if (_is_owner ())
			delete _tied_object ();
		InterfaceTiedBase <AbstractBase>::_tied_object (obj, release);
	}

protected:
	AbstractBaseTied (T* obj, Boolean release)
	{
		m_epv = &Skeleton <S, AbstractBase>::sm_epv.interface;
		_tied_object (obj, release);
	}

	~AbstractBaseTied ()
	{
		if (_is_owner ())
			delete _tied_object ();
	}
};

template <class T, class Primary, class ... Base> // Base includes all derived interfaces, directly and indirectly
class ImplementationTied :
	public AbstractBaseTied <T, ServantTied <T, Primary> >,
	public InterfaceTied <ServantTied <T, Primary>, Base> ...,
	public InterfaceTied <ServantTied <T, Primary>, Primary>
{
public:
	typedef Primary _PrimaryInterface;

	static const Char* _primary_interface ()
	{
		return Bridge <Primary>::_primary_interface ();
	}

	virtual Bridge <Interface>* _find_interface (const Char* id)
	{
		return Skeleton <ServantPOA <Primary>, Primary>::_find_interface (*this, id);
	}

	static Boolean ___is_a (const Char* type_id)
	{
		return Bridge <Primary>::___is_a (type_id);
	}

	T_ptr <Primary> _this ()
	{
		return InterfaceTiedBase <Primary>::_this ();
	}

protected:
	ImplementationTied (T* obj, Boolean release) :
		AbstractBaseTied <T, ServantTied <T, Primary> > (obj, release)
	{}
};

}
}

#endif
