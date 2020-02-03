#include <CORBA/TypeCode_c.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCodeException.h>

namespace CORBA {

DEFINE_USER_EXCEPTION (TypeCode::BadKind, CORBA_REPOSITORY_ID (TypeCode / BadKind));
const Nirvana::StaticInterface <TypeCode> TypeCode::_tc_BadKind = { STATIC_BRIDGE (Nirvana::TypeCodeException <TypeCode::BadKind>, TypeCode) };

DEFINE_USER_EXCEPTION (TypeCode::Bounds, CORBA_REPOSITORY_ID (TypeCode / Bounds));
const Nirvana::StaticInterface <TypeCode> TypeCode::_tc_Bounds = { STATIC_BRIDGE (Nirvana::TypeCodeException <TypeCode::Bounds>, TypeCode) };

namespace Nirvana {

const Char Bridge <TypeCode>::interface_id_[] = CORBA_REPOSITORY_ID (TypeCode);

}

}
