// Nirvana project
// Object Request Broker
// POA (virtual) interface implementation.
#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "Implementation.h"
#include "LifeCyclePOA.h"

namespace CORBA {
namespace Nirvana {

//! POA implementation of AbstractBase

template <>
class ServantPOA <AbstractBase> :
	public LifeCyclePOA,
	public InterfaceImplBase <ServantPOA <AbstractBase>, AbstractBase>
{
public:
	virtual Interface_ptr _query_interface (const String& id) = 0;

protected:
	ServantPOA ()
	{}
};

// POA implementation of PortableServer::ServantBase
template <>
class ServantPOA <PortableServer::ServantBase> :
	public virtual ServantPOA <AbstractBase>,
	public Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>,
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

	virtual Boolean _is_a (const String& type_id)
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
class ServantPOA <Object> :
	public virtual ServantPOA <PortableServer::ServantBase>,
	public InterfaceImplBase <ServantPOA <Object>, Object>,
	public LocalObjectLink
{
public:
	// Static overrides to resolve the ambiguity.
	static Interface* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env);
	static ABI_boolean __is_a (Bridge <Object>* obj, ABI_in <String> type_id, EnvironmentBridge* env);
	static ABI_boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env);

	// Delegate ReferenceCounter to AbstractBase
	virtual void _add_ref ()
	{
		ServantPOA <AbstractBase>::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		ServantPOA <AbstractBase>::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return ServantPOA <AbstractBase>::_refcount_value ();
	}

	// Object operations

	virtual ImplementationDef_ptr _get_implementation ()
	{
		return LocalObjectLink::_get_implementation ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return LocalObjectLink::_get_interface ();
	}

	virtual Boolean _is_a (const String& type_id)
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

	virtual Interface* _get_proxy ();
};

template <>
class ServantPOA <LocalObject> :
	public ServantPOA <Object>,
	public InterfaceImplBase <ServantPOA <LocalObject>, LocalObject>
{};

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
	virtual Interface_ptr _query_interface (const String& id)
	{
#ifdef _DEBUG
		Bridge <AbstractBase>* ab = this;
		const Bridge <AbstractBase>::EPV& epv = ab->_epv ();
		assert (!strcmp (epv.header.interface_id, Bridge <AbstractBase>::interface_id_));
#endif
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
