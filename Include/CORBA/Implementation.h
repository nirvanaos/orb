// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ObjectFactory_c.h"
#include "POA_c.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "DynamicServant_s.h"
#include "ReferenceCounter_s.h"
#include "Object_s.h"
#include "LocalObject_s.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

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
				throw MARSHAL ();
			return &static_cast <Bridge <Base>&> (S::_implementation (derived));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
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

class ReferenceCounterLink
{
public:
	void _add_ref ()
	{
		reference_counter_->_add_ref ();
	}

	void _remove_ref ()
	{
		reference_counter_->_remove_ref ();
	}

	ULong _refcount_value ()
	{
		return reference_counter_->_refcount_value ();
	}

protected:
	ReferenceCounterLink () :
		reference_counter_ (ReferenceCounter::_nil ())
	{}

	ReferenceCounterLink (DynamicServant_ptr dynamic);

	ReferenceCounterLink (ReferenceCounter_ptr rc) :
		reference_counter_ (rc)
	{}

	ReferenceCounterLink (const ReferenceCounterLink&) = delete;
	ReferenceCounterLink& operator = (const ReferenceCounterLink&)
	{
		return *this; // Do nothing
	}

	~ReferenceCounterLink ();

protected:
	ReferenceCounter_ptr reference_counter_;
};

//! Dynamic servant life cycle.
//! \tparam S Class implementing `_duplicate()' and `_release()' operations.
template <class S, class ... Bases>
class LifeCycleDynamic : public Bases...
{
public:
	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		try {
			return S::_duplicate (static_cast <Bridge <I>*> (itf));
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return nullptr;
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		try {
			S::_release (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

//! Life cycle with reference counting.
template <class S, class ... Bases>
class LifeCycleRefCnt :
	public LifeCycleDynamic <S, Bases...>
{
public:
	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		if (itf)
			S::_implementation (itf)._add_ref ();
		return itf;
	}

	template <class I>
	static void _release (Bridge <I>* itf)
	{
		if (itf)
			S::_implementation (itf)._remove_ref ();
	}
};

//! Non copyable reference.
template <class S, class ... Bases>
class LifeCycleNoCopy : public Bases...
{
public:
	template <class I>
	static Bridge <Interface>* __duplicate (Bridge <Interface>* itf, EnvironmentBridge* env)
	{
		env->set_exception (NO_IMPLEMENT ());
		return nullptr;
	}

	template <class I>
	static void __release (Bridge <Interface>* itf)
	{
		try {
			delete &S::_implementation (static_cast <Bridge <I>*> (itf));
		} catch (...) {
		}
	}
};

template <class S>
class DynamicServantImpl :
	public LifeCycleRefCnt <S>,
	public InterfaceImpl <S, ReferenceCounter>,
	public InterfaceImpl <S, DynamicServant>
{};

//! Dynamic servant for abstract interfaces
template <class S>
class LifeCycleRefCntAbstract :
	public DynamicServantImpl <S>,
	public ReferenceCounterLink
{
protected:
	LifeCycleRefCntAbstract () :
		ReferenceCounterLink (DynamicServant_ptr (this))
	{}
};

//! \brief Implements delegate to the core ServantBase implementation.
class ServantBaseLink :
	public Bridge <PortableServer::ServantBase>
{
public:
	operator Bridge <Object>& ()
	{
		return *Object_ptr (servant_base_);
	}

	// ServantBase operations

	PortableServer::POA_ptr _default_POA () const
	{
		return servant_base_->_default_POA ();
	}

	InterfaceDef_ptr _get_interface () const
	{
		return servant_base_->_get_interface ();
	}

	Boolean _is_a (const Char* type_id) const
	{
		return servant_base_->_is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return false;
	}

	// Our extension
	Boolean _is_active () const
	{
		return !servant_base_->_non_existent ();
	}

protected:
	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& epv) :
		Bridge <PortableServer::ServantBase> (epv),
		servant_base_ (PortableServer::ServantBase::_nil ())
	{}

	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& epv, DynamicServant_ptr dynamic) :
		Bridge <PortableServer::ServantBase> (epv)
	{
		_construct (dynamic);
	}

	ServantBaseLink (const ServantBaseLink&) = delete;
	ServantBaseLink& operator = (const ServantBaseLink&)
	{
		return *this; // Do nothing
	}

	void _construct (DynamicServant_ptr dynamic);

	void _implicitly_activate ();

protected:
	PortableServer::Servant servant_base_;
};

//! Standard implementation of PortableServer::ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, PortableServer::ServantBase> :
	public DynamicServantImpl <S>,
	public Skeleton <S, PortableServer::ServantBase>,
	public ServantBaseLink,
	public ReferenceCounterLink
{
protected:
	InterfaceImpl () :
		ServantBaseLink (Skeleton <S, PortableServer::ServantBase>::epv_, this),
		ReferenceCounterLink (ReferenceCounter_ptr (servant_base_))
	{}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}
};

//! Standard implementation of `CORBA::LocalObject'.

class LocalObjectLink
{
public:
	// Object operations

	ImplementationDef_ptr _get_implementation ()
	{
		return object_->_get_implementation ();
	}

	InterfaceDef_ptr _get_interface ()
	{
		return object_->_get_interface ();
	}

	Boolean _is_a (const Char* type_id)
	{
		return object_->_is_a (type_id);
	}

	Boolean _non_existent ()
	{
		return object_->_non_existent ();
	}

	Boolean _is_equivalent (Object_ptr other_object)
	{
		return object_->_is_equivalent (other_object);
	}

	ULong _hash (ULong maximum)
	{
		return object_->_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	LocalObjectLink () :
		object_ (Object::_nil ())
	{}

	LocalObjectLink (const LocalObjectLink&) = delete;
	LocalObjectLink& operator = (const LocalObjectLink&)
	{
		return *this; // Do nothing
	}

	ReferenceCounter_ptr _construct (AbstractBase_ptr base, DynamicServant_ptr dynamic);

private:
	Object_ptr object_;
};

//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, LocalObject> :
	public DynamicServantImpl <S>,
	public InterfaceImplBase <S, Object>,
	public InterfaceImplBase <S, LocalObject>,
	public LocalObjectLink,
	public ReferenceCounterLink
{
protected:
	InterfaceImpl () :
		ReferenceCounterLink (_construct (&static_cast <S&> (*this), this))
	{}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
	{}
};

//! \class	ImplementationSingle
//!
//! \brief	An implementation of single pseudo interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam	I	interface.

template <class S, class I>
class ImplementationSingle :
	public ServantTraits <S>,
	public LifeCycleRefCnt <S>,
	public InterfaceImpl <S, I>
{
protected:
	ImplementationSingle ()
	{}
};

//! \class	Implementation
//!
//! \brief	An implementation of interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam	Primary	Primary interface.
//! \tparam	Bases	 	All base interfaces derived directly or indirectly.
//! 								Don't include AbstractBase in base list.

template <class S, class Primary, class ... Bases>
class Implementation :
	public ServantTraits <S>,
	public InterfaceImpl <S, AbstractBase>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>
{
	class DummyActivator
	{
	public:
		static void _implicitly_activate ()
		{}
	};

public:
	Interface_ptr _query_interface (const Char* id)
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}

	T_ptr <Primary> _this ()
	{
		std::conditional < std::is_base_of <ServantBaseLink, Implementation <S, Primary, Bases...> >::value, 
			ServantBaseLink, DummyActivator>::type::_implicitly_activate ();
		static_cast <S&> (*this)._add_ref ();
		return this;
	}

protected:
	Implementation ()
	{}
};

}
}

namespace PortableServer {

template <typename Servant>
class Servant_var
{
protected:
	void swap (Servant* lhs, Servant* rhs)
	{
		Servant *tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}
public:
	Servant_var ()
		: _ptr (nullptr)
	{}

	Servant_var (Servant* p)
		: _ptr (p)
	{}

	Servant_var (const Servant_var& b)
		: _ptr (b._ptr)
	{
		if (_ptr)
			_ptr->_add_ref ();
	}

	~Servant_var ()
	{
		if (_ptr) {
			try {
				_ptr->_remove_ref ();
			} catch (...) {
				// swallow exceptions
			}
		}
	}

	Servant_var& operator = (Servant* p)
	{
		if (_ptr != p) {
			Servant_var <Servant> tmp = p;
			swap (_ptr, p);
		}
		return *this;
	}

	Servant_var& operator = (const Servant_var& b)
	{
		if (_ptr != b._ptr) {
			Servant_var <Servant> tmp = b;
			swap (_ptr, tmp._ptr);
		}
		return *this;
	}

	Servant* operator->() const
	{
		return _ptr;
	}

	Servant* in () const
	{
		return _ptr;
	}

	Servant*& inout ()
	{
		return _ptr;
	}

	Servant*& out ()
	{
		if (_ptr != 0) {
			Servant_var<Servant> tmp;
			swap (_ptr, tmp._ptr);
		}
		return _ptr;
	}

	Servant* _retn ()
	{
		Servant* retval = _ptr;
		_ptr = 0;
		return retval;
	}

private:
	Servant* _ptr;
};

}

#endif
