// Contains imports for all objects declared in the "Test_I1.h" header.
#include "Test_I1.h"
#include <Nirvana/OLF.h>

namespace Test {

extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_I1 = { OLF_IMPORT_INTERFACE, I1::repository_id_, ::CORBA::TypeCode::repository_id_ };
extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_SeqLong = { OLF_IMPORT_INTERFACE, "Test/_tc_SeqLong", ::CORBA::TypeCode::repository_id_ };
extern const ::Nirvana::ImportInterfaceT <::CORBA::TypeCode> _tc_MyException = { OLF_IMPORT_INTERFACE, MyException::repository_id_, ::CORBA::TypeCode::repository_id_ };

}
