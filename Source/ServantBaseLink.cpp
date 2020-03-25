#include <CORBA/ServantBaseLink.h>
#include <CORBA/POA.h>
#include <CORBA/core_objects.h>

namespace CORBA {
namespace Nirvana {

void ServantBaseLink::_construct ()
{
	servant_base_ = g_object_factory->create_servant (PortableServer::Servant (this));
}

Interface* ServantBaseLink::_get_proxy ()
{
	Interface* proxy = AbstractBase_ptr (servant_base_)->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

// In the final implementation this method won't be called for servants.
// So we could replace the implementation to throw_BAD_OPERATION() call.
Bridge <Object>* ServantBaseLink::_get_object (String_in iid) const
{
	return static_cast <Bridge <Object>*> (AbstractBase_ptr (servant_base_)->_query_interface (iid));
}

}
}