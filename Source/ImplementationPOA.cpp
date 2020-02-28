#include <CORBA/ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

ServantPOA <PortableServer::ServantBase>::ServantPOA () :
	ServantBaseLink (Skeleton <ServantPOA <PortableServer::ServantBase>, PortableServer::ServantBase>::epv_)
{}

void ServantPOA <PortableServer::ServantBase>::_construct ()
{
	ServantBaseLink::_construct (this);
	reference_counter_ = servant_base_;
}

Interface* ServantPOA <PortableServer::ServantBase>::_get_proxy ()
{
	_check_construct ();
	return ServantBaseLink::_get_proxy ();
}

ServantPOA <Object>::ServantPOA ()
{
	ServantPOA <AbstractBase>& ab = *this;
	reference_counter_ = LocalObjectLink::_construct (&ab, &ab);
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