#ifndef NIRVANA_ORB_TYPECODE_B_H_
#define NIRVANA_ORB_TYPECODE_B_H_

#include "Interface.h"

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

template <> struct Type <TCKind> : public TypeEnum <TCKind>
{
	static void check (TCKind tk)
	{
		if ((unsigned)tk > tk_local_interface)
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

template <>
class Bridge <TypeCode> :
	public BridgeMarshal <TypeCode>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Boolean (*equal) (Bridge <TypeCode>*, BridgeMarshal <TypeCode>*, EnvironmentBridge*);
			Boolean (*equivalent) (Bridge <TypeCode>*, BridgeMarshal <TypeCode>*, EnvironmentBridge*);
			BridgeMarshal <TypeCode>* (*get_compact_typecode) (Bridge <TypeCode>*, EnvironmentBridge*);
			TCKind (*kind) (Bridge <TypeCode>*, EnvironmentBridge*);
			const char* (*id) (Bridge <TypeCode>*, EnvironmentBridge*);
			const char* (*name) (Bridge <TypeCode>*, EnvironmentBridge*);
			ULong (*member_count) (Bridge <TypeCode>*, EnvironmentBridge*);
			const char* (*member_name) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
			BridgeMarshal <TypeCode>* (*member_type) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
			Any* (*member_label) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
			BridgeMarshal <TypeCode>* (*discriminator_type) (Bridge <TypeCode>*, EnvironmentBridge*);
			Long (*default_index) (Bridge <TypeCode>*, EnvironmentBridge*);
			ULong (*length) (Bridge <TypeCode>*, EnvironmentBridge*);
			BridgeMarshal <TypeCode>* (*content_type) (Bridge <TypeCode>*, EnvironmentBridge*);
			UShort (*fixed_digits) (Bridge <TypeCode>*, EnvironmentBridge*);
			Short (*fixed_scale) (Bridge <TypeCode>*, EnvironmentBridge*);
			Visibility (*member_visibility) (Bridge <TypeCode>*, ULong index, EnvironmentBridge*);
			ValueModifier (*type_modifier) (Bridge <TypeCode>*, EnvironmentBridge*);
			BridgeMarshal <TypeCode>* (*concrete_base_type) (Bridge <TypeCode>*, EnvironmentBridge*);

			ULong (*_size) (Bridge <TypeCode>*, EnvironmentBridge*);
			void (*_construct) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
			void (*_destruct) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
			void (*_copy) (Bridge <TypeCode>*, ::Nirvana::Pointer, ::Nirvana::ConstPointer, EnvironmentBridge*);
			void (*_move) (Bridge <TypeCode>*, ::Nirvana::Pointer, ::Nirvana::Pointer, EnvironmentBridge*);
			void (*_local_marshal) (Bridge <TypeCode>*, ::Nirvana::ConstPointer, ::Nirvana::Pointer, EnvironmentBridge*);
			void (*_local_unmarshal_in) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
			void (*_local_unmarshal_inout) (Bridge <TypeCode>*, ::Nirvana::Pointer, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const char interface_id_[];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <TypeCode> (epv.interface)
	{}
};

}
}

#endif
