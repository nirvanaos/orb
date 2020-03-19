#ifndef NIRVANA_ORB_CORE_TC_IMPEX_H_
#define NIRVANA_ORB_CORE_TC_IMPEX_H_

#include <Nirvana/OLF.h>

#define TC_IMPEX(t) namespace CORBA {\
extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##t = { 0, nullptr, nullptr, STATIC_BRIDGE (TC_##t, TypeCode) };\
} NIRVANA_EXPORT_INTERFACE1 (CORBA, "CORBA/_tc_" #t, TC_##t, CORBA::TypeCode);


#endif
