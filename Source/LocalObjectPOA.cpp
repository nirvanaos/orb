#include <CORBA/LocalObjectPOA.h>

namespace CORBA {
namespace Nirvana {

PortableServer::POA_var ServantPOA <LocalObject>::_default_POA ()
{
	return PortableServer::POA::_nil ();
}

InterfaceDef_var ServantPOA <LocalObject>::_get_interface ()
{
	_check_construct ();
	return LocalObjectLink::core_object_->_get_interface ();
}

Boolean ServantPOA <LocalObject>::_is_a (String_in type_id)
{
	_check_construct ();
	return LocalObjectLink::core_object_->_is_a (type_id);
}

Boolean ServantPOA <LocalObject>::_non_existent ()
{
	return LocalObjectLink::_non_existent ();
}

Interface* ServantPOA <LocalObject>::_get_proxy ()
{
	_check_construct ();
	return LocalObjectLink::_get_proxy ();
}

Bridge <Object>* ServantPOA <LocalObject>::_get_object (String_in iid)
{
	_check_construct ();
	return LocalObjectLink::_get_object (iid);
}

}
}
