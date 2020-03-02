#ifndef NIRVANA_ORB_CORE_PROXYOBJECT_H_
#define NIRVANA_ORB_CORE_PROXYOBJECT_H_

#include "ProxyBase.h"
#include "ObjectImpl.h"

namespace CORBA {
namespace Nirvana {
namespace Core {

class ProxyObject :
	public ProxyBase <ProxyObject, PortableServer::ServantBase>,
	public ObjectImpl
{
public:
	ProxyObject (PortableServer::Servant servant) :
		ProxyBase (servant)
	{}

	Boolean _is_equivalent (Object_ptr other) const
	{
		return default_is_equivalent (other);
	}
};

}
}
}

#endif
