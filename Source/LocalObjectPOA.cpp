#include <CORBA/LocalObjectPOA.h>

namespace CORBA {
namespace Nirvana {

PortableServer::POA_var ServantPOA <LocalObject>::_default_POA ()
{
	return PortableServer::POA::_nil ();
}

InterfaceDef_var ServantPOA <LocalObject>::_get_interface ()
{
	return ObjectLink::_get_interface ();
}

Boolean ServantPOA <LocalObject>::_is_a (const String& type_id)
{
	return ObjectLink::_is_a (type_id);
}

Boolean ServantPOA <LocalObject>::_non_existent ()
{
	return ObjectLink::_non_existent ();
}


}
}
