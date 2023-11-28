#include <CORBA/CORBA.h>
#include <CORBA/system_exceptions.h>
#include <Nirvana/OLF.h>

using namespace Nirvana;

namespace CORBA {

using namespace Internal;

#define IMPORT_TC(E) NIRVANA_IMPORT (_tc_##E, RepIdOf <E>::id, TypeCode)

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

}
