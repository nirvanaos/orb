// Nirvana project
// Object Request Broker
// POA Nirvana interface implementation
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

//! Virtual implementation of AbstractBase

template <>
class ServantPOA <AbstractBase> :
	public InterfaceImpl <ServantPOA <AbstractBase>, AbstractBase>,
	public ServantTraitsPOA,
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
	virtual const Char* _primary_interface () const = 0;

protected:
	ServantPOA ()
	{}
};

typedef ServantPOA <AbstractBase> AbstractBasePOA;

// Virtual implementation of ServantBase

template <>
class ServantPOA <ServantBase> :
	public virtual ServantPOA <AbstractBase>,
	public ServantBaseLinks,
	public Skeleton <ServantPOA <ServantBase>, ServantBase>
{
public:
	operator Bridge <Object>& ();

	operator Bridge <ServantBase>& ()
	{
		_check_links ();
		return static_cast <Bridge <ServantBase>&> (*this);
	}

	operator ServantLinks_ptr ()
	{
		_check_links ();
		return ServantBaseLinks::operator ServantLinks_ptr ();
	}

	// ServantBase operations

	virtual ::PortableServer::POA_ptr _default_POA ()
	{
		return ServantBaseLinks::_default_POA ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return ServantBaseLinks::_get_interface ();
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return ServantBaseLinks::_is_a (type_id);
	}

	virtual Boolean _non_existent ()
	{
		return ServantBaseLinks::_non_existent ();
	}

protected:
	ServantPOA () :
		ServantBaseLinks (Skeleton <ServantPOA <ServantBase>, ServantBase>::epv_)
	{}

	void _implicitly_activate ();

private:
	void _check_links ()
	{
		if (!servant_links_)
			_final_construct ();
	}

	void _final_construct ();
};

template <class Primary, class ... Bases>
class ImplementationPOA :
	public virtual ServantPOA <ServantBase>,
	public virtual ServantPOA <Bases>...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	virtual Interface_ptr _query_interface (const Char* id)
	{
		return FindInterface <Primary>::find (*this, id);
	}
	
	virtual const Char* _primary_interface () const
	{
		return Bridge <Primary>::interface_id_;
	}

	T_ptr <Primary> _this ()
	{
		_implicitly_activate ();
		return this;
	}

protected:
	ImplementationPOA ()
	{}
};

}
}

#endif
