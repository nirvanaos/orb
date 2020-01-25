#include "ObjectFactoryCore.h"
#include <Nirvana/OLF.h>

#pragma comment(linker, "/include:__exp_CORBA_Nirvana_object_factory")

extern "C" __declspec (allocate(OLF_BIND))
const Nirvana::ExportInterface _exp_CORBA_Nirvana_object_factory {
	Nirvana::OLF_EXPORT_INTERFACE, "CORBA/Nirvana/g_object_factory", STATIC_BRIDGE (CORBA::Nirvana::ObjectFactoryCore, CORBA::Nirvana::ObjectFactory)
};
