#include "ObjectFactory.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {
namespace Core {
StatelessCreationFrame* ObjectFactory::stateless_ = 0;
}
}
}

NIRVANA_EXPORT_INTERFACE2 (CORBA, Nirvana, g_object_factory, CORBA::Nirvana::Core::ObjectFactory, CORBA::Nirvana::ObjectFactory)
