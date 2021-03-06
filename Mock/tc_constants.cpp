#include <CORBA/CORBA.h>

#define IMPORT_TC(tc) const Nirvana::ImportInterfaceT <TypeCode> tc = { Nirvana::OLF_IMPORT_INTERFACE };

namespace CORBA {

IMPORT_TC (_tc_void)
IMPORT_TC (_tc_short)
IMPORT_TC (_tc_long)
IMPORT_TC (_tc_longlong)
IMPORT_TC (_tc_ushort)
IMPORT_TC (_tc_ulong)
IMPORT_TC (_tc_ulonglong)
IMPORT_TC (_tc_float)
IMPORT_TC (_tc_double)
IMPORT_TC (_tc_longdouble)
IMPORT_TC (_tc_boolean)
IMPORT_TC (_tc_char)
IMPORT_TC (_tc_wchar)
IMPORT_TC (_tc_octet)
IMPORT_TC (_tc_any)
IMPORT_TC (_tc_string)
IMPORT_TC (_tc_wstring)
IMPORT_TC (_tc_Object)
IMPORT_TC (_tc_TypeCode)
IMPORT_TC (_tc_ValueBase)

}

#undef IMPORT_TC
