#ifndef NIRVANA_ORB_CORE_LOCALOBJECT_H_
#define NIRVANA_ORB_CORE_LOCALOBJECT_H_

#include <CORBA/CORBA.h>
#include <CORBA/ImplementationPseudo.h>
#include "ProxyLocal.h"
#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

/// LocalObject default implementation
class LocalObject :
	public LifeCycleNoCopy <LocalObject>,
	public ImplementationPseudo <LocalObject, Object, AbstractBase>,
	public ObjectImpl
{
public:
	LocalObject (Object_ptr servant, Interface_ptr lifecycle) :
		proxy_manager_ (servant, lifecycle)
	{}

	Interface_ptr _query_interface (const String& iid)
	{
		return proxy_manager_._query_interface (iid);
	}

	// Object operations

	static Interface* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		return 0; // We dont implement it
	}

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
		return proxy_manager_.default_non_existent ();
	}

	Boolean _is_equivalent (Object_ptr other_object)
	{
		return proxy_manager_.default_is_equivalent (other_object);
	}

private:
	ProxyLocal proxy_manager_;
};

}
}
}

#endif