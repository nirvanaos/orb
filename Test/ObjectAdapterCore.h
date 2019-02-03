#ifndef NIRVANA_ORB_TEST_OBJECTADAPTERCORE_H_
#define NIRVANA_ORB_TEST_OBJECTADAPTERCORE_H_

#include "ServantLinksCore.h"
#include "ObjectCore.h"

namespace CORBA {
namespace Nirvana {

class ObjectAdapterCore :
	public AbstractBaseStatic <ObjectAdapterCore>,
	public InterfaceStatic <ObjectAdapterCore, ObjectAdapter>
{
public:
	static Bridge <Interface>* _find_interface (Bridge <AbstractBase>& base, const Char* id)
	{
		return Skeleton <ObjectAdapterCore, ObjectAdapter>::_find_interface (base, id);
	}

	static ServantLinks* create_servant (ServantBase_ptr servant, const Char* type_id)
	{
		return new ServantLinksCore (servant, type_id);
	}

	static void destroy_servant (ServantLinks* servant)
	{
		delete static_cast <ServantLinksCore*> (servant);
	}

	static void activate_object (ServantLinks* servant)
	{
		if (!servant->object) {
			ServantLinksCore* sl = static_cast <ServantLinksCore*> (servant);
			servant->object = new ObjectCore (sl->servant (), &(sl->object));
		}
	}
};

}
}

#endif
