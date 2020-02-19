// The Nirvana project.
// Object Request Broker.
// Standard Nirvana servant implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATION_H_
#define NIRVANA_ORB_IMPLEMENTATION_H_

#include <Nirvana/Nirvana.h>
#include "ServantImpl.h"
#include "POA.h"
#include "AbstractBase_s.h"
#include "ServantBase_s.h"
#include "Object_s.h"
#include "FindInterface.h"
#include "DynamicImpl.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

//! \brief Implements delegate to the core ServantBase implementation.
class ServantBaseLink :
	public Bridge <ServantBase>
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
	ServantBaseLink (const Bridge <ServantBase>::EPV& epv) :
		Bridge <ServantBase> (epv),
		servant_base_ (ServantBase::_nil ())
	{}

	ServantBaseLink (const ServantBaseLink&) = delete;
	ServantBaseLink& operator = (const ServantBaseLink&)
	{
		return *this; // Do nothing
	}

	void _construct ();

	Interface* _get_proxy ();

private:
	ServantBase_ptr servant ()
	{
		return ServantBase_ptr (&static_cast <ServantBase&> (static_cast <Bridge <ServantBase>&> (*this)));
	}

protected:
	ServantBase_ptr servant_base_;
};

//! Standard implementation of ServantBase.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, ServantBase> :
	public Skeleton <S, ServantBase>,
	public ServantBaseLink,
	public LifeCycleRefCnt <S>,
	public DynamicImpl <S>
{
protected:
	InterfaceImpl () :
		ServantBaseLink (Skeleton <S, ServantBase>::epv_)
	{
		_construct ();
	}

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

	void _construct (Bridge <AbstractBase>* base);

	Interface* _get_proxy (const Char* id = 0);

private:
	Object_ptr object_;
};

/// For LocalObject - based implementation, include CORBA::Object or CORBA::Nirvana::LocalObject in the list of base classes.
typedef Object LocalObject;

//! Standard implementation of local Object.
//! \tparam S Servant class implementing operations.
template <class S>
class InterfaceImpl <S, LocalObject> :
	public InterfaceImplBase <S, Object>,
	public LocalObjectLink,
	public LifeCycleRefCnt <S>,
	public DynamicImpl <S>
{
protected:
	InterfaceImpl ()
	{
		_construct (&static_cast <S&> (*this));
	}

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
	Interface_ptr _query_interface (const Char* id)
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
