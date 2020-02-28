#include <CORBA/ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

ServantPOA <PortableServer::ServantBase>::ServantPOA () :
	ServantBaseLink (Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>::epv_)
{}

Interface* ServantPOA <PortableServer::ServantBase>::_get_proxy ()
{
	_check_construct ();
	return ServantBaseLink::_get_proxy ();
}

ServantPOA <Object>::ServantPOA ()
{
	LocalObjectLink::_construct (this);
}

Interface* ServantPOA <Object>::_get_proxy ()
{
	return LocalObjectLink::_get_proxy ();
}

Interface* ServantPOA <Object>::__get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <Object>, Object>::__get_interface (obj, env);
}

ABI_boolean ServantPOA <Object>::__is_a (Bridge <Object>* obj, ABI_in <String> type_id, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <Object>, Object>::__is_a (obj, type_id, env);
}

ABI_boolean ServantPOA <Object>::__non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
{
	return Skeleton <ServantPOA <Object>, Object>::__non_existent (obj, env);
}

}
}