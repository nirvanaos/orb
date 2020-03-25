#include <CORBA/servant_core.h>

namespace CORBA {
namespace Nirvana {

Interface* get_proxy (PortableServer::Servant core_object)
{
	Interface* proxy = AbstractBase_ptr (core_object)->_query_interface (0);
	if (!proxy)
		::Nirvana::throw_MARSHAL ();
	return interface_duplicate (proxy);
}

}
}
