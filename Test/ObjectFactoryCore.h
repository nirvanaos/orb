#ifndef NIRVANA_TESTORB_OBJECTFACTORYCORE_H_
#define NIRVANA_TESTORB_OBJECTFACTORYCORE_H_

#include "Server.h"
#include <CORBA/ObjectFactory_s.h>
#include "ServantCore.h"
#include "LocalObjectCore.h"
#include "ReferenceCounterImpl.h"

namespace CORBA {
namespace Nirvana {

class ObjectFactoryCore :
	public ServantStatic <ObjectFactoryCore, ObjectFactory>
{
public:
	static PortableServer::Servant create_servant (PortableServer::Servant servant)
	{
#ifdef TEST_LOCAL_OBJECT
		assert (false);
#endif
		return (new ServantCore (servant))->_get_ptr ();
	}

	static Object_ptr create_local_object (Object_ptr servant)
	{
#ifndef TEST_LOCAL_OBJECT
		assert (false);
#endif
		return (new LocalObjectCore (servant))->_get_ptr ();
	}

	static ReferenceCounter_ptr create_reference_counter (DynamicServant_ptr dynamic)
	{
		return (new ReferenceCounterCore (dynamic))->_get_ptr ();
	}
};

}
}

#endif
