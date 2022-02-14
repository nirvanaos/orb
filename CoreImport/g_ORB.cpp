#include <CORBA/CORBA.h>
#include <CORBA/ORB.h>
#include <Nirvana/OLF.h>

namespace CORBA {

NIRVANA_OLF_SECTION
extern const Nirvana::ImportInterfaceT <ORB> g_ORB = {
	Nirvana::OLF_IMPORT_INTERFACE, "CORBA/g_ORB", ORB::repository_id_
};

}
