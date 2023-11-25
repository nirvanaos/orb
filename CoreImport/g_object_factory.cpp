#include <CORBA/Server.h>
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Internal {

extern const Nirvana::ImportInterfaceT <ObjectFactory> NIRVANA_OLF_SECTION (g_object_factory) = {
	Nirvana::OLF_IMPORT_INTERFACE,
	"CORBA/Internal/g_object_factory",
	CORBA::Internal::RepIdOf <ObjectFactory>::id
};

}
}
