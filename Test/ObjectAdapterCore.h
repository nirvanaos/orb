#ifndef NIRVANA_TESTORB_OBJECTADAPTERCORE_H_
#define NIRVANA_TESTORB_OBJECTADAPTERCORE_H_

#include "ObjectAdapter_s.h"
#include "ServantCore.h"
#include "LocalObjectCore.h"
#include <ImplementationStatic.h>

namespace CORBA {
namespace Nirvana {

class ObjectAdapterCore :
	public ImplementationStatic <ObjectAdapterCore, ObjectAdapter>
{
public:
	static PortableServer::Servant create_servant (PortableServer::Servant servant, DynamicServant_ptr dynamic)
	{
		return new ServantCore (servant, dynamic);
	}

	static Object_ptr create_local_object (DynamicServant_ptr servant)
	{
		return new LocalObjectCore (servant);
	}
};

}
}

#endif
