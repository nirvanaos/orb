#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>

namespace CORBA {

#define IMPORT_TC(E) NIRVANA_OLF_SECTION extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##E =\
{ ::Nirvana::OLF_IMPORT_INTERFACE, Nirvana::RepIdOf <E>::repository_id_, TypeCode::repository_id_ };

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

}
