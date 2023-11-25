#include <CORBA/CORBA.h>
#include <CORBA/ORB.h>
#include <Nirvana/OLF.h>

namespace CORBA {

extern const Nirvana::ImportInterfaceT <ORB> NIRVANA_OLF_SECTION (g_ORB) = {Nirvana::OLF_IMPORT_INTERFACE,
"CORBA/g_ORB", CORBA::Internal::RepIdOf <ORB>::id
};

}
