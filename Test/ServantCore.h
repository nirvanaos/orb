#ifndef NIRVANA_TESTORB_SERVANTCORE_H_
#define NIRVANA_TESTORB_SERVANTCORE_H_

#include "ObjectCore.h"
#include <CORBA/ServantBase_s.h>

namespace CORBA {
namespace Nirvana {

class ServantCore :
	public ServantTraits <ServantCore>,
	public Skeleton <ServantCore, AbstractBase>, // Derive only for _wide() implementation.
	public InterfaceImplBase <ServantCore, PortableServer::ServantBase>,
	public LifeCycleNoCopy <ServantCore>
{
public:
	ServantCore (PortableServer::Servant servant, DynamicServant_ptr dynamic) :
		object_ (servant, dynamic)
	{}

	operator Bridge <Object>& ()
	{
		return object_;
	}

	PortableServer::POA_ptr _default_POA () const;

	InterfaceDef_ptr _get_interface () const
	{
		return static_cast <const ObjectBase&> (object_)._get_interface ();
	}

	Boolean _is_a (const Char* type_id)
	{
		return static_cast <const ObjectBase&> (object_)._is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return !object_.is_active_;
	}

private:
	ObjectCore object_;
};

}
}

#endif
