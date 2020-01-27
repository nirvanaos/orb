// Nirvana project
// Object Request Broker
// POA (virtual) interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "Implementation.h"
#include <string.h>

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

	static ServantPOA <AbstractBase>& _implementation (Bridge <ReferenceCounter>* bridge);
	static ServantPOA <AbstractBase>& _implementation (Bridge <DynamicServant>* bridge);
	static ServantPOA <LocalObject>& _implementation (Bridge <Object>* bridge);
};

//! POA implementation of AbstractBase

template <>
class ServantPOA <AbstractBase> :
	public ServantTraitsPOA,
	public InterfaceImplBase <ServantPOA <AbstractBase>, AbstractBase>,
	public InterfaceImplBase <ServantPOA <AbstractBase>, ReferenceCounter>,
	public InterfaceImplBase <ServantPOA <AbstractBase>, DynamicServant>,
	public LifeCycleRefCnt <ServantPOA <AbstractBase> >,
	public ReferenceCounterLink
{
public:
	virtual void _add_ref ()
	{
		ReferenceCounterLink::_add_ref ();
	}

	virtual void _remove_ref ()
	{
		ReferenceCounterLink::_remove_ref ();
	}

	virtual ULong _refcount_value ()
	{
		return ReferenceCounterLink::_refcount_value ();
	}

	virtual Interface_ptr _query_interface (const Char* id) = 0;

	virtual ~ServantPOA ()
	{}

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

	virtual Bridge <Interface>* _get_proxy ();

private:
	friend class ServantTraitsPOA;

	void _check_construct ()
	{
		if (!ServantBaseLink::servant_base_)
			_construct ();
	}

	void _construct ();
};

template <>
class ServantPOA <LocalObject> :
	public virtual ServantPOA <PortableServer::ServantBase>,
	public InterfaceImplBase <ServantPOA <LocalObject>, Object>,
	public InterfaceImplBase <ServantPOA <LocalObject>, LocalObject>,
	public LocalObjectLink
{
public:
	// Static overrides to resolve the ambiguity.
	static BridgeMarshal <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env);
	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, EnvironmentBridge* env);
	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env);

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

	virtual Bridge <Interface>* _get_proxy ();
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
#ifdef _DEBUG
		Bridge <AbstractBase>* ab = this;
		const Bridge <AbstractBase>::EPV& epv = ab->_epv ();
		assert (!strcmp (epv.interface.interface_id, Bridge <AbstractBase>::interface_id_));
#endif
		return FindInterface <Primary, Bases...>::find (static_cast <ServantPOA <Primary>&> (*this), id);
	}

	T_ptr <Primary> _this ()
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
