#ifndef NIRVANA_ORB_CORE_OBJECTS_H_
#define NIRVANA_ORB_CORE_OBJECTS_H_

#include "ObjectFactory.h"
#include "LocalMarshal.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Nirvana {

extern const ::Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory;
extern const ::Nirvana::ImportInterfaceT <LocalMarshal> g_local_marshal;

}
}

#endif
