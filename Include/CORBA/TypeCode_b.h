#ifndef NIRVANA_ORB_TYPECODE_B_H_
#define NIRVANA_ORB_TYPECODE_B_H_

#include "Bridge.h"
#include "TypeEnum.h"
#include <Nirvana/throw_exception.h>

namespace CORBA {

class Any;
class TypeCode;

enum TCKind
{
	tk_null, tk_void,
	tk_short, tk_long, tk_ushort, tk_ulong,
	tk_float, tk_double, tk_boolean, tk_char,
	tk_octet, tk_any, tk_TypeCode, tk_Principal, tk_objref,
	tk_struct, tk_union, tk_enum, tk_string,
	tk_sequence, tk_array, tk_alias, tk_except,
	tk_longlong, tk_ulonglong, tk_longdouble,
	tk_wchar, tk_wstring, tk_fixed,
	tk_value, tk_value_box,
	tk_native,
	tk_abstract_interface,
	tk_local_interface
};

namespace Nirvana {

template <> struct Type <TCKind> : TypeEnum <TCKind>
{
	static void check (TypeEnum <TCKind>::ABI_type tk)
	{
		if (tk > tk_local_interface)
			::Nirvana::throw_BAD_PARAM ();
	}
};

}

typedef Short Visibility;
const Visibility PRIVATE_MEMBER = 0;
const Visibility PUBLIC_MEMBER = 1;

typedef Short ValueModifier;
const ValueModifier VM_NONE = 0;
const ValueModifier VM_CUSTOM = 1;
const ValueModifier VM_ABSTRACT = 2;
const ValueModifier VM_TRUNCATABLE = 3;

namespace Nirvana {

BRIDGE_BEGIN (TypeCode)
Boolean (*equal) (Bridge <TypeCode>*, Interface*, EnvironmentBridge*);
Boolean (*equivalent) (Bridge <TypeCode>*, Interface*, EnvironmentBridge*);
Interface* (*get_compact_typecode) (Bridge <TypeCode>*, EnvironmentBridge*);
ABI_Enum (*kind) (Bridge <TypeCode>*, EnvironmentBridge*);
const char* (*id) (Bridge <TypeCode>*, EnvironmentBridge*);
const char* (*name) (Bridge <TypeCode>*, EnvironmentBridge*);
ULong (*member_count) (Bridge <TypeCode>*, EnvironmentBridge*);
const char* (*member_name) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
Interface* (*member_type) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
const Any* (*member_label) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
Interface* (*discriminator_type) (Bridge <TypeCode>*, EnvironmentBridge*);
Long (*default_index) (Bridge <TypeCode>*, EnvironmentBridge*);
ULong (*length) (Bridge <TypeCode>*, EnvironmentBridge*);
Interface* (*content_type) (Bridge <TypeCode>*, EnvironmentBridge*);
UShort (*fixed_digits) (Bridge <TypeCode>*, EnvironmentBridge*);
Short (*fixed_scale) (Bridge <TypeCode>*, EnvironmentBridge*);
Visibility (*member_visibility) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
ValueModifier (*type_modifier) (Bridge <TypeCode>*, EnvironmentBridge*);
Interface* (*concrete_base_type) (Bridge <TypeCode>*, EnvironmentBridge*);

ULong (*_size) (Bridge <TypeCode>*, EnvironmentBridge*);
void (*_construct) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
void (*_destruct) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
void (*_copy) (Bridge <TypeCode>*, ::Nirvana::Pointer, ::Nirvana::ConstPointer, EnvironmentBridge*);
void (*_move) (Bridge <TypeCode>*, ::Nirvana::Pointer, ::Nirvana::Pointer, EnvironmentBridge*);
void (*_local_marshal) (Bridge <TypeCode>*, ::Nirvana::ConstPointer, ::Nirvana::Pointer, EnvironmentBridge*);
void (*_local_unmarshal_in) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
void (*_local_unmarshal_inout) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
BRIDGE_END ()

}
}

#endif
