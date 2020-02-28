// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include <Nirvana/Nirvana.h>
#include "POA.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "Object_s.h"
#include "LocalObject_s.h"
#include "LifeCycleServant.h"
#include "FindInterface.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

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

	Boolean _is_a (const String& type_id) const
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

	Interface* _get_proxy ();

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
	public Skeleton <S, PortableServer::ServantBase>,
	public ServantBaseLink,
	public LifeCycleServant <S>
{
protected:
	InterfaceImpl () :
		ServantBaseLink (Skeleton <S, PortableServer::ServantBase>::epv_, this),
		LifeCycleServant <S> (servant_base_)
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

	Boolean _is_a (const String& type_id)
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

	Interface* _get_proxy ();

private:
	Object_ptr object_;
};

//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, LocalObject> :
	public LifeCycleServant <S>,
	public InterfaceImplBase <S, Object>,
	public InterfaceImplBase <S, LocalObject>,
	public LocalObjectLink
{
protected:
	InterfaceImpl () :
		LifeCycleServant <S> (_construct (&static_cast <S&> (*this), this))
	{}

	InterfaceImpl (const InterfaceImpl&) :
		InterfaceImpl ()
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
	Interface_ptr _query_interface (const String& id)
	{
		return FindInterface <Primary, Bases...>::find (static_cast <S&> (*this), id);
	}

	I_ptr <Primary> _this ()
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

#endif
