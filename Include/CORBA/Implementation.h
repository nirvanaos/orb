// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include "ServantImpl.h"
#include "ObjectFactory.h"
#include "POA.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "DynamicServant_s.h"
#include "ReferenceCounter_s.h"
#include "Object_s.h"
#include "LocalObject_s.h"
#include "FindInterface.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

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

	ReferenceCounterLink (Bridge <DynamicServant>* dynamic);

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

template <class S>
class DynamicServantImpl :
	public LifeCycleRefCnt <S>,
	public InterfaceImpl <S, ReferenceCounter>,
	public InterfaceImpl <S, DynamicServant>
{};

//! Dynamic servant for pseudo interfaces
template <class S>
class LifeCycleRefCntPseudo :
	public DynamicServantImpl <S>,
	public ReferenceCounterLink
{
protected:
	LifeCycleRefCntPseudo () :
		ReferenceCounterLink (this)
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

	ServantBaseLink (const Bridge <PortableServer::ServantBase>::EPV& epv, Bridge <DynamicServant>* dynamic) :
		Bridge <PortableServer::ServantBase> (epv)
	{
		_construct (dynamic);
	}

	ServantBaseLink (const ServantBaseLink&) = delete;
	ServantBaseLink& operator = (const ServantBaseLink&)
	{
		return *this; // Do nothing
	}

	void _construct (Bridge <DynamicServant>* dynamic);

	Bridge <Interface>* _get_proxy ();

private:
	PortableServer::Servant servant ()
	{
		return PortableServer::Servant (&static_cast <PortableServer::ServantBase&> (static_cast <Bridge <PortableServer::ServantBase>&> (*this)));
	}

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

	ReferenceCounter_ptr _construct (Bridge <AbstractBase>* base, Bridge <DynamicServant>* dynamic);

	Bridge <Interface>* _get_proxy ();

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

//! \class ImplementationPseudo
//!
//! \brief An implementation of a pseudo interface.
//!
//! You also have to derive your servant from some life cycle implementation.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.

template <class S, class Primary, class ... Bases>
class ImplementationPseudo :
	public ServantTraits <S>,
	public InterfaceImplBase <S, Bases> ...,
	public InterfaceImplBase <S, Primary>
{
public:

	//! \fn T_ptr <Primary> _get_ptr ()
	//!
	//! \brief Gets the pointer.
	//!   Works like _this() method but doesn't increment the reference counter.
	//!
	//! \return The primary interface pointer.

	T_ptr <Primary> _get_ptr ()
	{
		return T_ptr <Primary> (&static_cast <Primary&> (static_cast <Bridge <Primary>&> (*this)));
	}

protected:
	ImplementationPseudo ()
	{}
};

//! \class Implementation
//!
//! \brief An implementation of interface.
//!
//! \tparam S Servant class implementing operations.
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.
//!    Don't include AbstractBase in base list.

template <class S, class Primary, class ... Bases>
class Implementation :
	public ServantTraits <S>,
	public InterfaceImpl <S, AbstractBase>,
	public InterfaceImpl <S, Bases>...,
	public InterfaceImpl <S, Primary>
{
public:
	Interface_ptr _query_interface (const Char* id)
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}

	T_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (
			std::conditional <std::is_base_of <LocalObjectLink, Implementation <S, Primary, Bases...> >::value,
			LocalObjectLink, ServantBaseLink>::type::
			_get_proxy ());
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

	Servant* operator -> () const
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
