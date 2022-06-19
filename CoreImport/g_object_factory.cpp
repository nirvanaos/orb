#include <CORBA/Server.h>
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Internal {

NIRVANA_OLF_SECTION
extern const Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory = {
	Nirvana::OLF_IMPORT_INTERFACE,
	"CORBA/Internal/g_object_factory",
	CORBA::Internal::RepIdOf <ObjectFactory>::id_
};

}
}
