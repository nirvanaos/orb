#ifndef NIRVANA_TESTORB_OBJECTADAPTERCORE_H_
#define NIRVANA_TESTORB_OBJECTADAPTERCORE_H_

#include "ObjectAdapter_s.h"
#include "ServantLinksCore.h"

namespace CORBA {
namespace Nirvana {

class ObjectAdapterCore :
	public ImplementationStatic <ObjectAdapterCore, ObjectAdapter, AbstractBase>
{
public:
	static ServantLinks_ptr create_servant (ServantBase_ptr servant, const Char* type_id)
	{
		return new ServantLinksCore (servant, type_id);
	}

	static Object_ptr create_local_object (AbstractBase_ptr base, const Char* type_id)
	{
		throw NO_IMPLEMENT ();
	}
};

}
}

#endif
