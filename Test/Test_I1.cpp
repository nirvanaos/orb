// Contains imports for all objects declared in the "Test_I1.h" header.
#include "Test_I1.h"
#include <Nirvana/OLF.h>

namespace Test {

DEFINE_USER_EXCEPTION (MyException, "IDL:Test/MyException:1.0")

__declspec (allocate (OLF_BIND)) extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode>
_tc_SeqLong = { ::Nirvana::OLF_IMPORT_INTERFACE, "Test/_tc_SeqLong", ::CORBA::TypeCode::repository_id_ };

__declspec (allocate (OLF_BIND)) extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode>
_tc_I1 = { ::Nirvana::OLF_IMPORT_INTERFACE, I1::repository_id_, ::CORBA::TypeCode::repository_id_ };

}
