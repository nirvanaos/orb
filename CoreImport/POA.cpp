#include <CORBA/POA.h>
#include <Nirvana/OLF.h>

namespace PortableServer {

using namespace ::CORBA;
using namespace ::Nirvana;

NIRVANA_OLF_SECTION
const ImportInterfaceT <TypeCode> POA::_tc_ServantAlreadyActive = { OLF_IMPORT_INTERFACE, POA::ServantAlreadyActive::repository_id_, TypeCode::repository_id_ };

NIRVANA_OLF_SECTION
const ImportInterfaceT <TypeCode> POA::_tc_ObjectNotActive = { OLF_IMPORT_INTERFACE, POA::ObjectNotActive::repository_id_, TypeCode::repository_id_ };

}
