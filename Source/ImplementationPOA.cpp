#include <CORBA/ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

ServantPOA <ServantBase>::ServantPOA () :
	ServantBaseLink (Skeleton <ServantPOA <ServantBase>, ServantBase>::epv_)
{}

Interface* ServantPOA <ServantBase>::_get_proxy ()
{
	_check_construct ();
	return ServantBaseLink::_get_proxy ();
}

ServantPOA <Object>::ServantPOA ()
{
	LocalObjectLink::_construct (this);
}

Interface* ServantPOA <LocalObject>::_get_proxy ()
{
	return LocalObjectLink::_get_proxy ();
}
/*
Interface* ServantPOA <LocalObject>::__get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <LocalObject>, Object>::__get_interface (obj, env);
}

Boolean ServantPOA <LocalObject>::__is_a (Bridge <Object>* obj, const Char* type_id, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <LocalObject>, Object>::__is_a (obj, type_id, env);
}

Boolean ServantPOA <LocalObject>::__non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <LocalObject>, Object>::__non_existent (obj, env);
}
*/
}
}