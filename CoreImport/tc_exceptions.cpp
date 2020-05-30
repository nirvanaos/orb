#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>

namespace CORBA {

#define IMPORT_TC(E) __declspec (allocate (OLF_BIND)) extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##E =\
{ ::Nirvana::OLF_IMPORT_INTERFACE, E::repository_id_, TypeCode::repository_id_ };

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

}
