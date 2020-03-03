#include <CORBA/ObjectLink.h>
#include <CORBA/core_objects.h>

namespace CORBA {
namespace Nirvana {

ObjectLink::ObjectLink (const Bridge <Object>::EPV& epv) :
	Bridge <Object> (epv),
	object_ (g_object_factory->create_local_object (
		Object_ptr (&static_cast <Object&> (static_cast <Bridge <Object>&> (*this)))))
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
	return static_cast <Bridge <Object>*> (AbstractBase_ptr (object_)->_query_interface (iid));
}

}
}
