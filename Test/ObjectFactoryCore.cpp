#include "ObjectFactoryCore.h"
#include <Nirvana/OLF.h>

#if defined _M_AMD64
#pragma comment(linker, "/include:_exp_CORBA_Nirvana_object_factory")
#else
#pragma comment(linker, "/include:__exp_CORBA_Nirvana_object_factory")
#endif

extern "C" __declspec (allocate(OLF_BIND))
const Nirvana::ExportInterface _exp_CORBA_Nirvana_object_factory {
	Nirvana::OLF_EXPORT_INTERFACE, "CORBA/Nirvana/g_object_factory", STATIC_BRIDGE (CORBA::Nirvana::ObjectFactoryCore, CORBA::Nirvana::ObjectFactory)
};

namespace CORBA {
namespace Nirvana {

__declspec (allocate(OLF_BIND))
const ::Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory = {
	::Nirvana::OLF_IMPORT_INTERFACE, "CORBA/Nirvana/g_object_factory", ObjectFactory::interface_id_
};

}
}
