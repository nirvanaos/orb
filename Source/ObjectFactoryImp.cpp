#include <CORBA/ObjectFactory_c.h>
#include <CORBA/OLF.h>

namespace CORBA {
namespace Nirvana {

__declspec (allocate(OLF_BIND))
const ImportInterfaceT <ObjectFactory> g_object_factory = {
	OLF_IMPORT_INTERFACE, "CORBA/Nirvana/g_object_factory", ObjectFactory::interface_id_
};

}
}
