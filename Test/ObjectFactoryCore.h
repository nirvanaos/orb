#ifndef NIRVANA_TESTORB_OBJECTFACTORYCORE_H_
#define NIRVANA_TESTORB_OBJECTFACTORYCORE_H_

#include "CORBA/ObjectFactory_s.h"
#include "ServantCore.h"
#include "LocalObjectCore.h"
#include "ReferenceCounterImpl.h"
#include <CORBA/ImplementationStatic.h>

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

	static LocalObject_ptr create_local_object (AbstractBase_ptr base, DynamicServant_ptr dynamic)
	{
#ifndef TEST_LOCAL_OBJECT
		assert (false);
#endif
		return new LocalObjectCore (base, dynamic);
	}

	static ReferenceCounter_ptr create_reference_counter (DynamicServant_ptr dynamic)
	{
		return new ReferenceCounterCore (dynamic);
	}
};

}
}

#endif