#ifndef NIRVANA_ORB_SERVANT_CORE_H_
#define NIRVANA_ORB_SERVANT_CORE_H_

#include "ServantBase.h"

namespace CORBA {
namespace Nirvana {

// In the final implementation this method won't be called for servants.
// So we could replace the implementation to throw_BAD_OPERATION() call
// or to assert (false) and return nullptr.
inline Bridge <Object>* get_object_from_core (PortableServer::Servant core_object, String_in iid)
{
	return static_cast <Bridge <Object>*> (AbstractBase_ptr (core_object)->_query_interface (iid));
}

Interface* get_proxy (PortableServer::Servant core_object);

}
}

#endif
