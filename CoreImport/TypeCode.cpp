#include <CORBA/TypeCode.h>
#include <Nirvana/OLF.h>

namespace CORBA {

__declspec (allocate(OLF_BIND))
extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_TCKind = { ::Nirvana::OLF_IMPORT_INTERFACE, Nirvana::RepIdOf <TCKind>::repository_id_, TypeCode::repository_id_ };

__declspec (allocate(OLF_BIND))
const ::Nirvana::ImportInterfaceT <TypeCode> TypeCode::_tc_BadKind = { ::Nirvana::OLF_IMPORT_INTERFACE, TypeCode::BadKind::repository_id_, TypeCode::repository_id_ };

__declspec (allocate(OLF_BIND))
const ::Nirvana::ImportInterfaceT <TypeCode> TypeCode::_tc_Bounds = { ::Nirvana::OLF_IMPORT_INTERFACE, TypeCode::Bounds::repository_id_, TypeCode::repository_id_ };

}
