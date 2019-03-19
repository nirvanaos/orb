#include <Implementation.h>

namespace CORBA {
namespace Nirvana {

void ServantBaseLink::_construct (Bridge <DynamicServant>& dynamic_servant)
{
	assert (!servant_base_);
	servant_base_ = g_object_adapter->create_servant (this, &dynamic_servant);
}

void ServantBaseLink::_implicitly_activate ()
{
	if (!_is_active ()) {
		::PortableServer::POA_var poa = PortableServer::Servant (this)->_default_POA ();
		poa->activate_object (servant_base_);
	}
}

LocalObjectLink::LocalObjectLink (DynamicServant_ptr servant) :
	object_ (g_object_adapter->create_local_object (servant))
{}

}
}