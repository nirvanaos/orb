#include <CORBA/ObjectLink.h>
#include <CORBA/core_objects.h>

namespace CORBA {
namespace Nirvana {

ObjectLink::ObjectLink (const Bridge <LocalObject>::EPV& epv, Bridge <AbstractBase>& ab) :
	Bridge <LocalObject> (epv),
	object_ (g_object_factory->create_local_object (
		LocalObject_ptr (&static_cast <LocalObject&> (static_cast <Bridge <LocalObject>&> (*this))),
		AbstractBase_ptr (&static_cast <AbstractBase&> (ab))))
{}

Interface* ObjectLink::_get_proxy ()
{
	Interface* proxy = AbstractBase_ptr (object_)->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

Bridge <Object>* ObjectLink::_get_object (String_in iid) const
{
	if (RepositoryId::compatible (object_->_epv ().header.interface_id, iid))
		return &Object_ptr (object_);
	else
		::Nirvana::throw_INV_OBJREF ();
}

}
}
