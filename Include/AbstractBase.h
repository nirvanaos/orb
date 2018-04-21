#ifndef NIRVANA_ORB_ABSTRACTBASE_H_
#define NIRVANA_ORB_ABSTRACTBASE_H_

#include "Interface.h"
#include "RefCount.h"

namespace CORBA {

typedef Nirvana::T_ptr <AbstractBase> AbstractBase_ptr;

namespace Nirvana {

// AbstractBase

template <>
class Bridge <AbstractBase> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Interface>* (*find_interface) (Bridge <AbstractBase>*, const Char*, Environment*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (const EPV&)Bridge <Interface>::_epv ();
	}

protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
	{}
};

template <class T>
class Client <T, AbstractBase> :
	public ClientBase <T, AbstractBase>
{
public:
	Bridge <Interface>* _find_interface (const Char* type_id);
};

template <class T>
Bridge <Interface>* Client <T, AbstractBase>::_find_interface (const Char* type_id)
{
	Environment env;
	Bridge <AbstractBase>& bridge = _bridge ();
	Bridge <Interface>* ret = (bridge._epv ().epv.find_interface) (&bridge, type_id, &env);
	env.check ();
	return ret;
}

}

class AbstractBase :
	public Nirvana::ClientInterface <AbstractBase>
{
public:
	typedef AbstractBase_ptr _ptr_type;

	inline Object_ptr _to_object ();
};

}

namespace PortableServer {

namespace Nirvana {

using namespace CORBA;
using namespace CORBA::Nirvana;

extern void _check_pointer (const void* p);
extern void _check_pointer (const Bridge <Interface>* obj, const Bridge <Interface>::EPV& epv);

//! Servant part of an interface
template <class S, class I> class Skeleton;

template <class S>
class Skeleton <S, AbstractBase>
{
public:
	static const typename Bridge <AbstractBase>::EPV sm_epv;

protected:
	static Bridge <Interface>* __find_interface (Bridge <AbstractBase>* base, const Char* id, Environment* env)
	{
		try {
			_check_pointer (base, sm_epv.interface);
			_check_pointer (id);
			return S::_find_interface (*base, id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <AbstractBase>::EPV Skeleton <S, AbstractBase>::sm_epv = {
	{	// interface
		S::_duplicate <AbstractBase>,
		S::_release <AbstractBase>
	},
	{	// epv
		S::__find_interface
	}
};

// Implementation

template <class S> class AbstractBaseVirtual;
template <class I> class InterfaceVirtual;
template <class S, class I> class ServantVirtual;

// Virtual implementation

template <>
class InterfaceVirtual <AbstractBase> :
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
	friend class AbstractBaseVirtual;

	InterfaceVirtual (const EPV& epv, void* implementation) :
		Bridge <AbstractBase> (epv),
		m_implementation (implementation)
	{}

	InterfaceVirtual ()
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
	static InterfaceVirtual <AbstractBase>& _get_base (Bridge <Interface>* itf);
	static InterfaceVirtual <AbstractBase>& _get_base (Bridge <AbstractBase>* itf);

private:
	void* m_implementation;
};

template <class I>
class InterfaceVirtualBase :
	public Bridge <I>,
	public virtual InterfaceVirtual <AbstractBase>
{
protected:
	InterfaceVirtualBase (const typename Bridge <I>::EPV& epv) :
		Bridge <I> (epv)
	{}

	InterfaceVirtualBase ()
	{}

	T_ptr <I> _this ()
	{
		return static_cast <Bridge <I>*> (this);
	}
};

template <class I>
Bridge <Interface>* InterfaceVirtual <AbstractBase>::_duplicate (Bridge <Interface>* itf, Environment* env)
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
Bridge <Interface>* InterfaceVirtual <AbstractBase>::_duplicate <AbstractBase> (Bridge <Interface>* itf, Environment* env)
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
void InterfaceVirtual <AbstractBase>::_release (Bridge <Interface>* itf)
{
	try {
		_get_base (itf)._remove_ref ();
	} catch (...) {
	}
}

template <>
void InterfaceVirtual <AbstractBase>::_release <AbstractBase> (Bridge <Interface>* itf)
{
	try {
		_get_base (static_cast <Bridge <AbstractBase>*> (itf))._remove_ref ();
	} catch (...) {
	}
}

template <class S>
class AbstractBaseVirtual :
	public virtual InterfaceVirtual <AbstractBase>,
	public Skeleton <S, AbstractBase>
{
public:
	template <class I>
	static S& _implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <S, I>::sm_epv.interface);
		return *reinterpret_cast <S*> (static_cast <InterfaceVirtual <I>&> (*bridge)._implementation ());
	}

	static S& _implementation (Bridge <AbstractBase>& base)
	{
		return *reinterpret_cast <S*> (static_cast <InterfaceVirtual <AbstractBase>&> (base)._implementation ());
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
			return static_cast <Bridge <Base>*> (static_cast <InterfaceVirtual <Derived>*> (itf));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

protected:
	AbstractBaseVirtual (S& implementation) :
		InterfaceVirtual <AbstractBase> (sm_epv, &implementation)
	{}
};

}
}

#endif
