#include <CORBA/CORBA.h>
#include <CORBA/system_exceptions.h>

namespace CORBA {

#define IMPORT_TC(E) const Nirvana::ImportInterfaceT <TypeCode> _tc_##E = { Nirvana::OLF_IMPORT_INTERFACE };

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

}
