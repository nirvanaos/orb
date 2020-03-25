#ifndef NIRVANA_ORB_LOCAL_CORE_H_
#define NIRVANA_ORB_LOCAL_CORE_H_

#include "LocalObject.h"

namespace CORBA {
namespace Nirvana {

Bridge <Object>* get_object_from_core (LocalObject_ptr core_object, String_in iid);

Interface* get_proxy (LocalObject_ptr core_object);

}
}

#endif
