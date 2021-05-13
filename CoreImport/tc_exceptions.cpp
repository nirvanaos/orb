#include <CORBA/CORBA.h>
#include <CORBA/system_exceptions.h>
#include <Nirvana/OLF.h>

using namespace Nirvana;

namespace CORBA {

using namespace Internal;

#define IMPORT_TC(E) NIRVANA_OLF_SECTION extern const ImportInterfaceT <TypeCode> _tc_##E = { OLF_IMPORT_INTERFACE, RepIdOf <E>::repository_id_, TypeCode::repository_id_ };

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

}
