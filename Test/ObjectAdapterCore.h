#ifndef NIRVANA_TESTORB_OBJECTADAPTERCORE_H_
#define NIRVANA_TESTORB_OBJECTADAPTERCORE_H_

#include "ObjectAdapter_s.h"
#include "ServantCore.h"
#include <ImplementationStatic.h>

namespace CORBA {
namespace Nirvana {

class ObjectAdapterCore :
	public ImplementationStatic <ObjectAdapterCore, ObjectAdapter>
{
public:
	static ServantBase_ptr create_servant (ServantBase_ptr servant, DynamicServant_ptr dynamic)
	{
		return new ServantCore (servant, dynamic);
	}

	static Object_ptr create_local_object (DynamicServant_ptr dynamic)
	{
		throw NO_IMPLEMENT ();
	}
};

}
}

#endif
