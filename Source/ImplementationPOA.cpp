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

Bridge <Interface>* ServantPOA <PortableServer::ServantBase>::_implicitly_activate ()
{
	_check_construct ();
	return ServantBaseLink::_implicitly_activate ();
}

ServantPOA <LocalObject>::ServantPOA ()
{
	ServantPOA <AbstractBase>& ab = *this;
	reference_counter_ = LocalObjectLink::_construct (&ab, &ab);
}

Bridge <Interface>* ServantPOA <LocalObject>::_implicitly_activate ()
{
	return LocalObjectLink::_implicitly_activate ();
}

BridgeMarshal <InterfaceDef>* ServantPOA <LocalObject>::__get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
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