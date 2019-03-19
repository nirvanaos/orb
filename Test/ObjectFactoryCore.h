#ifndef NIRVANA_TESTORB_OBJECTFACTORYCORE_H_
#define NIRVANA_TESTORB_OBJECTFACTORYCORE_H_

#include "ObjectFactory_s.h"
#include "ServantCore.h"
#include "LocalObjectCore.h"
#include <ImplementationStatic.h>

namespace CORBA {
namespace Nirvana {

class ObjectFactoryCore :
	public ImplementationStatic <ObjectFactoryCore, ObjectFactory>
{
public:
	static PortableServer::Servant create_servant (PortableServer::Servant servant, DynamicServant_ptr dynamic)
	{
#ifdef TEST_LOCAL_OBJECT
		assert (false);
#endif
		return new ServantCore (servant, dynamic);
	}

	static Object_ptr create_local_object (DynamicServant_ptr servant)
	{
#ifndef TEST_LOCAL_OBJECT
		assert (false);
#endif
		return new LocalObjectCore (servant);
	}
};

}
}

#endif
