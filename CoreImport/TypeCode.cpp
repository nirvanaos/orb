#include <CORBA/TypeCode.h>
#include <Nirvana/OLF.h>

namespace CORBA {

using namespace ::Nirvana;

NIRVANA_OLF_SECTION
extern const ImportInterfaceT <TypeCode> _tc_TCKind = { OLF_IMPORT_INTERFACE, Nirvana::RepIdOf <TCKind>::repository_id_, TypeCode::repository_id_ };

NIRVANA_OLF_SECTION
const ImportInterfaceT <TypeCode> TypeCode::_tc_BadKind = { OLF_IMPORT_INTERFACE, TypeCode::BadKind::repository_id_, TypeCode::repository_id_ };

NIRVANA_OLF_SECTION
const ImportInterfaceT <TypeCode> TypeCode::_tc_Bounds = { OLF_IMPORT_INTERFACE, TypeCode::Bounds::repository_id_, TypeCode::repository_id_ };

}
