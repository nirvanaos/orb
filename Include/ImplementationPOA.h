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

	virtual Interface_ptr _query_interface (const Char* id)
	{
		return Interface_ptr::nil ();
	}

	virtual const Char* _primary_interface () const = 0;

protected:
	ServantPOA ()
	{}

	virtual void _implicitly_activate ()
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

	virtual Interface_ptr _query_interface (const Char* id);

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

	virtual void _implicitly_activate ();

private:
	void _check_links ()
	{
		if (!servant_links_)
			_final_construct ();
	}

	void _final_construct ();
};

struct InterfaceEntryPOA
{
	InterfaceEntry base;
	Bridge <Interface>* (*find_base) (Bridge <Interface>* servant, const Char*);

	static Bridge <Interface>* find (const InterfaceEntryPOA* begin, const InterfaceEntryPOA* end, void* servant, const Char* id);
};

template <class Primary, class ... I>
class InterfaceFinderPOA
{
	template <class Itf>
	static Bridge <Interface>* cast (void* servant)
	{
		return &static_cast <Bridge <Itf>&> (*reinterpret_cast <ServantPOA <Primary>*> (servant));
	}

	template <class Itf>
	static Bridge <Interface>* find_base (Bridge <Interface>* bridge, const Char* id)
	{
		return static_cast <ServantPOA <Itf>&> (static_cast <Bridge <Itf>&> (*bridge)).ServantPOA <Itf>::_query_interface (id);
	}

	template <>
	static Bridge <Interface>* find_base <Primary> (Bridge <Interface>* bridge, const Char* id)
	{
		return nullptr;
	}

	template <>
	static Bridge <Interface>* find_base <Object> (Bridge <Interface>* bridge, const Char* id)
	{
		return nullptr;
	}

public:
	static Bridge <Interface>* find (ServantPOA <Primary>& servant, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <Primary>::interface_id_, id))
			return Interface::_duplicate (&static_cast <Bridge <Primary>&> (servant));

		static const InterfaceEntryPOA table [] = {
			{ { Bridge <I>::interface_id_, cast <I> }, find_base <I> }...,
		};

		return InterfaceEntryPOA::find (table, table + sizeof (table) / sizeof (*table), &servant, id);
	}
};

template <class Primary, class ... Bases>
class ImplementationPOA :
	public virtual ServantPOA <Bases>...,
	public InterfaceImpl <ServantPOA <Primary>, Primary>
{
public:
	virtual Interface_ptr _query_interface (const Char* id)
	{
		return InterfaceFinderPOA <Primary, Bases...>::find (static_cast <ServantPOA <Primary>&> (*this), id);
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
