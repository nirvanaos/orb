#ifndef NIRVANA_TESTORB_SERVANTCORE_H_
#define NIRVANA_TESTORB_SERVANTCORE_H_

#include "Server.h"
#include "ObjectCore.h"
#include <CORBA/ReferenceCounter.h>

namespace CORBA {
namespace Nirvana {

class ServantCore :
	public ImplementationPseudo <ServantCore, PortableServer::ServantBase>,
	public LifeCycleNoCopy <ServantCore>
{
public:
	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, const Char* id, EnvironmentBridge* env)
	{
		return ServantTraits <ServantCore>::_wide <Base, Derived> (derived, id, env);
	}

	template <>
	static Bridge <ReferenceCounter>* _wide <ReferenceCounter, PortableServer::ServantBase> (Bridge <PortableServer::ServantBase>* derived, const Char* id, EnvironmentBridge* env)
	{
		return 0;
	}

	ServantCore (PortableServer::Servant servant) :
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
