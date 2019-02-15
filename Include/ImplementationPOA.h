// Nirvana project
// Object Request Broker
// POA Nirvana interface implementation
#ifndef NIRVANA_ORB_IMPLEMENTATIONPOA_H_
#define NIRVANA_ORB_IMPLEMENTATIONPOA_H_

#include "Implementation.h"

namespace CORBA {
namespace Nirvana {

template <class I> class ServantPOA;

// Virtual implementation of AbstractBase

template <>
class ServantPOA <AbstractBase> :
	public InterfaceImpl <ServantPOA <AbstractBase>, AbstractBase>,
	public RefCountBase
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

	template <class I>
	static Bridge <I>* _duplicate (Bridge <I>* itf)
	{
		_servant (itf)._add_ref ();
		return itf;
	}

	template <class I>
	static void _release (Bridge <I>* itf)
	{
		_servant (itf)._remove_ref ();
	}

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

	virtual Interface_ptr _find_interface (const Char* id) = 0;
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
	// ServantBase operations

	virtual POA_ptr _default_POA ()
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
};

}
}

#endif
