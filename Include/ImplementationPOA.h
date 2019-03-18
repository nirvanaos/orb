// Nirvana project
// Object Request Broker
// POA (virtual) interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "Implementation.h"

namespace CORBA {
namespace Nirvana {

template <class I> class ServantPOA;

class ServantTraitsPOA
{
public:
	template <class I>
	static ServantPOA <I>& _servant (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <ServantPOA <I>, I>::epv_.interface);
		return static_cast <ServantPOA <I>&> (*bridge);
	}

	template <class I>
	static ServantPOA <I>& _implementation (Bridge <I>* bridge)
	{
		return _servant (bridge);
	}
};

//! POA implementation of AbstractBase

template <>
class ServantPOA <AbstractBase> :
	public ServantTraitsPOA,
	public InterfaceImplBase <ServantPOA <AbstractBase>, AbstractBase>,
	public LifeCycleRefCnt <ServantPOA <AbstractBase> >
{
public:
	virtual void _add_ref ()
	{
		RefCountBase::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		RefCountBase::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return RefCountBase::_refcount_value ();
	}

	virtual Interface_ptr _query_interface (const Char* id) = 0;

protected:
	ServantPOA ()
	{}

	virtual void _implicitly_activate ()
	{}
};

//! CORBA::Nirvana::DynamicServant interface
template <>
class ServantPOA <DynamicServant> :
	public virtual ServantPOA <AbstractBase>,
	public InterfaceImplBase <ServantPOA <DynamicServant>, DynamicServant>
{
public:
	virtual const Char* _primary_interface () const = 0;
};

// POA implementation of PortableServer::ServantBase
template <>
class ServantPOA <PortableServer::ServantBase> :
	public virtual ServantPOA <DynamicServant>,
	public Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>,
	public ServantBaseLink
{
public:
	// ServantBase operations

	virtual PortableServer::POA_ptr _default_POA ()
	{
		return ServantBaseLink::_default_POA ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return ServantBaseLink::_get_interface ();
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return ServantBaseLink::_is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		return ServantBaseLink::_non_existent ();
	}

protected:
	ServantPOA () :
		ServantBaseLink (Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>::epv_, *this)
	{}

	virtual void _implicitly_activate ()
	{
		ServantBaseLink::_implicitly_activate ();
	}
};

/*
template <>
class ServantPOA <LocalObject> :
	public virtual ServantPOA <Object>,
	public LocalObjectLink,
	public InterfaceImplBase <ServantPOA <LocalObject>, Object>
{
public:
	// Object operations

	virtual ImplementationDef_ptr _get_implementation ()
	{
		return LocalObjectLink::_get_implementation ();
	}

	InterfaceDef_ptr _get_interface ()
	{
		return LocalObjectLink::_get_interface ();
	}

	Boolean _is_a (const Char* type_id)
	{
		return LocalObjectLink::_is_a (type_id);
	}

	Boolean _non_existent ()
	{
		return LocalObjectLink::_non_existent ();
	}

	Boolean _is_equivalent (Object_ptr other_object)
	{
		return LocalObjectLink::_is_equivalent (other_object);
	}

	ULong _hash (ULong maximum)
	{
		return LocalObjectLink::_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	ServantPOA () :
		LocalObjectLink (Skeleton <ServantPOA <DynamicServant>, DynamicServant>::epv_)
	{}
};
*/
//! \class	ImplementationPOA
//!
//! \brief	Portable implementation of interface.
//!
//! \tparam	Primary	Primary interface.
//! \tparam	Bases	 	All base interfaces derived directly or indirectly.
//! 								Don't include AbstractBase in base list.

template <class Primary, class ... Bases>
class ImplementationPOA :
	public virtual ServantPOA <Bases>...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	virtual Interface_ptr _query_interface (const Char* id)
	{
		return Interface::_duplicate (FindInterface <Primary, Bases...>::find (static_cast <ServantPOA <Primary>&> (*this), id));
	}
	
	virtual const Char* _primary_interface () const
	{
		return Bridge <Primary>::interface_id_;
	}

	T_ptr <Primary> _this ()
	{
		this->_implicitly_activate ();
		return this;
	}

protected:
	ImplementationPOA ()
	{}
};

}
}

#endif
