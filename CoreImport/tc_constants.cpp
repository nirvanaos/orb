#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>

namespace CORBA {

#define IMPORT_BY_NAME(t) NIRVANA_OLF_SECTION_N (_tc_##t) extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##t \
{ ::Nirvana::OLF_IMPORT_INTERFACE, "CORBA/_tc_" #t, TypeCode::repository_id_ }

#define IMPORT_BY_ID(obj) NIRVANA_OLF_SECTION_N (_tc_##obj) extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##obj \
{ ::Nirvana::OLF_IMPORT_INTERFACE, obj::repository_id_, TypeCode::repository_id_ }

IMPORT_BY_NAME (void);
IMPORT_BY_NAME (short);
IMPORT_BY_NAME (long);
IMPORT_BY_NAME (longlong);
IMPORT_BY_NAME (ushort);
IMPORT_BY_NAME (ulong);
IMPORT_BY_NAME (ulonglong);
IMPORT_BY_NAME (float);
IMPORT_BY_NAME (double);
IMPORT_BY_NAME (longdouble);
IMPORT_BY_NAME (boolean);
IMPORT_BY_NAME (char);
IMPORT_BY_NAME (wchar);
IMPORT_BY_NAME (octet);
IMPORT_BY_NAME (any);
IMPORT_BY_NAME (string);
IMPORT_BY_NAME (wstring);

IMPORT_BY_ID (TypeCode);
IMPORT_BY_ID (Object);
IMPORT_BY_ID (ValueBase);

}
