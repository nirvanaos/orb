#ifndef NIRVANA_ORB_LOCALOBJECTPOA_H_
#define NIRVANA_ORB_LOCALOBJECTPOA_H_

#include "ServantBasePOA.h"
#include "LocalObjectLink.h"
#include "LocalObject_s.h"

#ifdef _MSC_BUILD
#pragma warning (disable:4250)
#endif

namespace CORBA {
namespace Nirvana {

template <>
class NIRVANA_NOVTABLE ServantPOA <LocalObject> :
	public virtual ServantPOA <PortableServer::ServantBase>,
	public Skeleton <ServantPOA <LocalObject>, LocalObject>,
	public LocalObjectLink
{
public:
	virtual Bridge <Object>* _get_object (String_in iid);

	// Object operations

	virtual PortableServer::POA_var _default_POA ();
	virtual InterfaceDef_var _get_interface ();
	virtual Boolean _is_a (const String& type_id);
	virtual Boolean _non_existent ();

	using Skeleton <ServantPOA <LocalObject>, LocalObject>::__non_existent;

protected:
	ServantPOA () :
		LocalObjectLink (Skeleton <ServantPOA <LocalObject>, LocalObject>::epv_)
	{}

	virtual Interface* _get_proxy ();

private:
	void _check_construct ()
	{
		if (!LocalObjectLink::core_object_)
			LocalObjectLink::_construct (*this);
	}
};

}
}

#endif
