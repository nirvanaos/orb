#ifndef NIRVANA_ORB_SERVANTBASEPOA_H_
#define NIRVANA_ORB_SERVANTBASEPOA_H_

#include "AbstractBasePOA.h"
#include "ServantBaseLink.h"

namespace CORBA {
namespace Nirvana {

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

}
}

#endif
