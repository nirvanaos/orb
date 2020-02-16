#include <CORBA/ImplementationPOA.h>

namespace CORBA {
namespace Nirvana {

ServantPOA <AbstractBase>& ServantTraitsPOA::_implementation (Bridge <ReferenceCounter>* bridge)
{
	return __implementation <ReferenceCounter, AbstractBase> (bridge);
}

ServantPOA <AbstractBase>& ServantTraitsPOA::_implementation (Bridge <DynamicServant>* bridge)
{
	return __implementation <DynamicServant, AbstractBase> (bridge);
}

ServantPOA <LocalObject>& ServantTraitsPOA::_implementation (Bridge <Object>* bridge)
{
	return __implementation <Object, LocalObject> (bridge);
}

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

ServantPOA <LocalObject>::ServantPOA ()
{
	ServantPOA <AbstractBase>& ab = *this;
	reference_counter_ = LocalObjectLink::_construct (&ab, &ab);
}

Interface* ServantPOA <LocalObject>::_get_proxy ()
{
	return LocalObjectLink::_get_proxy ();
}

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

}
}