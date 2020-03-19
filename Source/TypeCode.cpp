#include <CORBA/TypeCode.h>
#include <Nirvana/OLF.h>

namespace CORBA {

DEFINE_CORBA_INTERFACE_EXCEPTION (CORBA, TypeCode, BadKind);

DEFINE_CORBA_INTERFACE_EXCEPTION (CORBA, TypeCode, Bounds);

extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_TCKind = { ::Nirvana::OLF_IMPORT_INTERFACE, Nirvana::RepIdOf <TCKind>::repository_id_, TypeCode::repository_id_ };

}
