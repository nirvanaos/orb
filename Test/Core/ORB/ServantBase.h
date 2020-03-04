#ifndef NIRVANA_ORB_CORE_SERVANTBASE_H_
#define NIRVANA_ORB_CORE_SERVANTBASE_H_

#include <CORBA/ImplementationPseudo.h>
#include "ProxyObject.h"
#include <CORBA/ReferenceCounter.h>

namespace CORBA {
namespace Nirvana {
namespace Core {

/// ServantBase default implementation
class ServantBase :
	public LifeCycleNoCopy <ServantBase>,
	public ImplementationPseudo <ServantBase, PortableServer::ServantBase, AbstractBase>
{
public:
	template <class Base, class Derived>
	static Bridge <Base>* _wide (Bridge <Derived>* derived, String_in id, EnvironmentBridge* env)
	{
		return ServantTraits <ServantBase>::_wide <Base, Derived> (derived, id, env);
	}

	template <>
	static Bridge <ReferenceCounter>* _wide <ReferenceCounter, PortableServer::ServantBase> (Bridge <PortableServer::ServantBase>* derived, String_in id, EnvironmentBridge* env)
	{
		return 0;
	}

	ServantBase (PortableServer::Servant servant) :
		proxy_manager_ (servant)
	{}

	PortableServer::Servant __core_servant ()
	{
		return this;
	}

	Object_ptr proxy_manager ()
	{
		return proxy_manager_._get_ptr ();
	}

	Interface_ptr _query_interface (const String& iid)
	{
		return proxy_manager_._query_interface (iid);
	}

	// ServantBase default implementation

	PortableServer::POA_var _default_POA () const;

	InterfaceDef_var _get_interface () const
	{
		return proxy_manager_.default_get_interface ();
	}

	Boolean _is_a (const String& type_id) const
	{
		return proxy_manager_.default_is_a (type_id);
	}

	Boolean _non_existent () const
	{
		return false;
	}

private:
	ProxyObject proxy_manager_;
};

}
}
}

#endif
