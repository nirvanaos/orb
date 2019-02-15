#ifndef NIRVANA_TESTORB_OBJECTADAPTERCORE_H_
#define NIRVANA_TESTORB_OBJECTADAPTERCORE_H_

#include "ObjectAdapter_s.h"
#include "ServantLinksCore.h"

namespace CORBA {
namespace Nirvana {

class ObjectAdapterCore :
	public AbstractBaseStatic <ObjectAdapterCore>,
	public InterfaceStatic <ObjectAdapterCore, ObjectAdapter>
{
public:
	Interface_ptr _find_interface (const Char* id)
	{
		return FindInterface <ObjectAdapter>::find (*this, id);
	}

	static ServantLinks* create_servant (ServantBase_ptr servant, const Char* type_id)
	{
		return new ServantLinksCore (servant, type_id);
	}

	static void destroy_servant (ServantLinks* servant)
	{
		delete static_cast <ServantLinksCore*> (servant);
	}

	static Object_ptr create_local_object (AbstractBase_ptr base, const Char* type_id)
	{
		return Object::_nil ();
	}
};

}
}

#endif
