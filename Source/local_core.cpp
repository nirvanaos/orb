#include <CORBA/local_core.h>

namespace CORBA {
namespace Nirvana {

Bridge <Object>* get_object_from_core (LocalObject_ptr core_object, String_in iid)
{
	Bridge <LocalObject>* bridge = &core_object;
	Environment env;
	Bridge <Object>* obj = bridge->_epv ().base.CORBA_Object (bridge, iid, &env);
	env.check ();
	return obj;
}

Interface* get_proxy (LocalObject_ptr core_object)
{
	Interface* proxy = AbstractBase_ptr (Object_ptr (core_object))->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

}
}
