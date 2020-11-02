#ifndef NIRVANA_ORB_SERVANTBASEPOA_H_
#define NIRVANA_ORB_SERVANTBASEPOA_H_

#include "AbstractBasePOA.h"
#include "ServantBaseLink.h"
#include "ServantBase_s.h"

namespace CORBA {
namespace Nirvana {

// POA implementation of PortableServer::ServantBase
template <>
class NIRVANA_NOVTABLE ServantPOA <PortableServer::ServantBase> :
	public virtual ServantPOA <AbstractBase>,
	public Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>,
	public ServantBaseLink
{
public:
	virtual Bridge <Object>* _get_object (String_in iid)
	{
		return ServantBaseLink::_get_object (iid);
	}

	// ServantBase operations

	virtual PortableServer::POA_var _default_POA ()
	{
		_check_construct ();
		return ServantBaseLink::_default_POA ();
	}

	virtual InterfaceDef_var _get_interface ()
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
		return false;
	}

protected:
	ServantPOA ();

	virtual Interface* _get_proxy ();

private:
	friend class ServantTraitsPOA;

	void _check_construct ()
	{
		if (!ServantBaseLink::core_object_)
			_construct ();
	}
};

}
}

#endif
