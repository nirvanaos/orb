#include <CORBA/CORBA.h>
#include <CORBA/ObjectFactory.h>
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Internal {

NIRVANA_OLF_SECTION
extern const Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory = {
	Nirvana::OLF_IMPORT_INTERFACE, "CORBA/Internal/g_object_factory", ObjectFactory::repository_id_
};

}
}
