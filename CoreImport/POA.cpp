#include <CORBA/POA.h>
#include <Nirvana/OLF.h>

namespace PortableServer {

using namespace ::CORBA;
using namespace ::Nirvana;

__declspec (allocate(OLF_BIND))
const ImportInterfaceT <TypeCode> POA::_tc_ServantAlreadyActive = { OLF_IMPORT_INTERFACE, POA::ServantAlreadyActive::repository_id_, TypeCode::repository_id_ };

__declspec (allocate(OLF_BIND))
const ImportInterfaceT <TypeCode> POA::_tc_ObjectNotActive = { OLF_IMPORT_INTERFACE, POA::ObjectNotActive::repository_id_, TypeCode::repository_id_ };

}
