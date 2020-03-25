#include <CORBA/LocalObjectLink.h>
#include <CORBA/core_objects.h>

namespace CORBA {
namespace Nirvana {

LocalObjectLink::LocalObjectLink (const Bridge <LocalObject>::EPV& epv, Bridge <AbstractBase>& ab) :
	Bridge <LocalObject> (epv),
	core_object_ (g_object_factory->create_local_object (
		LocalObject_ptr (&static_cast <LocalObject&> (static_cast <Bridge <LocalObject>&> (*this))),
		AbstractBase_ptr (&static_cast <AbstractBase&> (ab))))
{}

}
}
