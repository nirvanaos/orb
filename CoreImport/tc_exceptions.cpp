#include <CORBA/CORBA.h>
#include <CORBA/system_exceptions.h>
#include <Nirvana/OLF.h>

using namespace Nirvana;

namespace CORBA {

using namespace Internal;

#define IMPORT_TC(E) extern const ImportInterfaceT <TypeCode> NIRVANA_OLF_SECTION (_tc_##E) = {\
OLF_IMPORT_INTERFACE, RepIdOf <E>::id, RepIdOf <TypeCode>::id };

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

}
