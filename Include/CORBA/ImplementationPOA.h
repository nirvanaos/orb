// Nirvana project
// Object Request Broker
// POA (virtual) interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "Implementation.h"
#include "Servant_var.h"

namespace CORBA {
namespace Nirvana {

template <class I> class ServantPOA;

class ServantTraitsPOA :
	public ServantTraits <ServantTraitsPOA>
{
public:
	template <class I, class IS>
	static ServantPOA <IS>& __implementation (Bridge <I>* bridge)
	{
		_check_pointer (bridge, Skeleton <ServantPOA <IS>, I>::epv_.interface);
		return static_cast <ServantPOA <IS>&> (*bridge);
	}

	template <class I>
	static ServantPOA <I>& _implementation (Bridge <I>* bridge)
	{
		return __implementation <I, I> (bridge);
	}
};

//! POA implementation of AbstractBase

template <>
class ServantPOA <AbstractBase> :
	public ServantTraitsPOA,
	public InterfaceImplBase <ServantPOA <AbstractBase>, AbstractBase>,
	public LifeCycleRefCnt <ServantPOA <AbstractBase> >,
	public DynamicImpl <ServantPOA <AbstractBase> >
{
public:
	virtual void _add_ref ()
	{
		DynamicImpl <ServantPOA <AbstractBase> >::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		DynamicImpl <ServantPOA <AbstractBase> >::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return DynamicImpl <ServantPOA <AbstractBase> >::_refcount_value ();
	}

	virtual Interface_ptr _query_interface (const Char* id) = 0;

	virtual ~ServantPOA ()
	{}

protected:
	ServantPOA ()
	{}
};

// POA implementation of ServantBase
template <>
class ServantPOA <ServantBase> :
	public virtual ServantPOA <AbstractBase>,
	public Skeleton <ServantPOA <ServantBase>, ServantBase>,
	public ServantBaseLink
{
public:
	virtual void _add_ref ()
	{
		_check_construct ();
		ServantPOA <AbstractBase>::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		_check_construct ();
		ServantPOA <AbstractBase>::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		_check_construct ();
		return ServantPOA <AbstractBase>::_refcount_value ();
	}

	// ServantBase operations

	virtual PortableServer::POA_ptr _default_POA ()
	{
		_check_construct ();
		return ServantBaseLink::_default_POA ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		_check_construct ();
		return ServantBaseLink::_get_interface ();
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		_check_construct ();
		return ServantBaseLink::_is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		_check_construct ();
		return ServantBaseLink::_non_existent ();
	}

protected:
	ServantPOA ();

	ServantPOA (const ServantPOA&) :
		ServantPOA ()
	{}

	virtual Interface* _get_proxy ();

private:
	friend class ServantTraitsPOA;

	void _check_construct ()
	{
		if (!ServantBaseLink::servant_base_)
			_construct ();
	}
};

template <>
class ServantPOA <LocalObject> :
	public virtual ServantPOA <AbstractBase>,
	public InterfaceImplBase <ServantPOA <LocalObject>, Object>,
	public LocalObjectLink
{
public:
	// Object operations

	virtual ImplementationDef_ptr _get_implementation ()
	{
		return LocalObjectLink::_get_implementation ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return LocalObjectLink::_get_interface ();
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return LocalObjectLink::_is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		return LocalObjectLink::_non_existent ();
	}

	virtual Boolean _is_equivalent (Object_ptr other_object)
	{
		return LocalObjectLink::_is_equivalent (other_object);
	}

	virtual ULong _hash (ULong maximum)
	{
		return LocalObjectLink::_hash (maximum);
	}
	// TODO: Other Object operations shall be here...

protected:
	ServantPOA ();

	ServantPOA (const ServantPOA&) :
		ServantPOA ()
	{}

	virtual Interface* _get_proxy ();
};

//! \class ImplementationPOA
//!
//! \brief Portable implementation of interface.
//!
//! \tparam Primary Primary interface.
//! \tparam Bases All base interfaces derived directly or indirectly.
//!               Don't include AbstractBase in base list.

template <class Primary, class ... Bases>
class ImplementationPOA :
	public virtual ServantPOA <Bases>...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	virtual Interface_ptr _query_interface (const Char* id)
	{
		return FindInterface <Primary, Bases...>::find (static_cast <ServantPOA <Primary>&> (*this), id);
	}

	I_ptr <Primary> _this ()
	{
		return static_cast <Primary*> (this->_get_proxy ());
	}

protected:
	ImplementationPOA ()
	{}
};

}
}

#endif
