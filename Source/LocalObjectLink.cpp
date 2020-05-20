#include <CORBA/LocalObjectLink.h>
#include <CORBA/core_objects.h>

namespace CORBA {
namespace Nirvana {

void LocalObjectLink::_construct (Bridge <AbstractBase>& ab)
{
	core_object_ = g_object_factory->create_local_object (this, AbstractBase_ptr (&static_cast <AbstractBase&> (ab)));
}

}
}
