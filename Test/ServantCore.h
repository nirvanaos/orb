#ifndef NIRVANA_TESTORB_SERVANTCORE_H_
#define NIRVANA_TESTORB_SERVANTCORE_H_

#include <CORBA/ServantBase_s.h>
#include "ObjectCore.h"
#include "ReferenceCounterImpl.h"

namespace CORBA {
namespace Nirvana {

class ServantCore :
	public ImplementationPseudo <ServantCore, ServantBase>,
	public LifeCycleNoCopy <ServantCore>
{
public:
	I_ptr <ServantBase> _get_ptr ()
	{
		return this;
	}

	ServantCore (ServantBase_ptr servant) :
		object_ (servant)
	{}

	operator Bridge <Object>& ()
	{
		return object_;
	}

	operator Bridge <AbstractBase>& ()
	{
		return object_;
	}

	PortableServer::POA_ptr _default_POA () const;

	InterfaceDef_ptr _get_interface () const
	{
		return static_cast <const ObjectBase&> (object_)._get_interface ();
	}

	Boolean _is_a (const String& type_id)
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
