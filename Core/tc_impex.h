#ifndef NIRVANA_ORB_CORE_TC_IMPEX_H_
#define NIRVANA_ORB_CORE_TC_IMPEX_H_

#include <Nirvana/OLF.h>

#define TC_IMPEX_EX(T, id) namespace CORBA {\
extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##T = { 0, nullptr, nullptr, STATIC_BRIDGE (TC_##T, TypeCode) };\
} NIRVANA_EXPORT_INTERFACE1 (CORBA, id, TC_##T, CORBA::TypeCode);

// Import and export for type code
#define TC_IMPEX(T) TC_IMPEX_EX (T, "CORBA/_tc_" #T)
#define TC_IMPEX_BY_ID(T) TC_IMPEX_EX (T, CORBA::TC_##T::RepositoryType::repository_id_)

// Import and export for interface exception
#define INTERFACE_EXC_IMPEX(I, E) namespace CORBA {\
const ::Nirvana::ImportInterfaceT <TypeCode> I::_tc_##E = { 0, 0, 0, STATIC_BRIDGE (TC_##I##_##E, TypeCode) };\
} NIRVANA_EXPORT_INTERFACE1 (CORBA, CORBA::I::E::repository_id_, TC_##I##_##E, CORBA::TypeCode);

#endif