#include <CORBA/TypeCode.h>
#include <CORBA/RepositoryId.h>
#include <CORBA/TypeCodeException.h>

#include <Nirvana/OLF.h>
#include <CORBA/TypeCodeBasic.h>

namespace CORBA {

DEFINE_INTERFACE_EXCEPTION (TypeCode, BadKind, CORBA_REPOSITORY_ID (TypeCode/BadKind));
//const Nirvana::StaticI_ptr <TypeCode> TypeCode::_tc_BadKind { STATIC_BRIDGE (Nirvana::TypeCodeException <TypeCode::BadKind>, TypeCode) };

DEFINE_INTERFACE_EXCEPTION (TypeCode, Bounds, CORBA_REPOSITORY_ID (TypeCode/Bounds));
//const Nirvana::StaticI_ptr <TypeCode> TypeCode::_tc_Bounds { STATIC_BRIDGE (Nirvana::TypeCodeException <TypeCode::Bounds>, TypeCode) };

}
