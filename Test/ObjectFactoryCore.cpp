#include "ObjectFactoryCore.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {
StatelessCreationFrame* ObjectFactoryCore::stateless_ = 0;
}
}

#ifdef TEST_MACRO
__pragma(comment (linker, "/include:" C_NAME_PREFIX "_exp_CORBA_Nirvana_g_object_factory"))\
extern "C" __declspec (allocate(OLF_BIND))\
const Nirvana::ExportInterface _exp_CORBA_Nirvana_g_object_factory { Nirvana::OLF_EXPORT_INTERFACE, "CORBA/Nirvana/g_object_factory", STATIC_BRIDGE (CORBA::Nirvana::ObjectFactoryCore, CORBA::Nirvana::ObjectFactory) };
#else
NIRVANA_EXPORT_INTERFACE2 (CORBA, Nirvana, g_object_factory, CORBA::Nirvana::ObjectFactoryCore, CORBA::Nirvana::ObjectFactory)
#endif
