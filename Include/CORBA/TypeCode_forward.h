#ifndef NIRVANA_ORB_TYPECODE_FORWARD_H_
#define NIRVANA_ORB_TYPECODE_FORWARD_H_
#include "Client.h"
#include "TypeEnum.h"
#include "UserException.h"
#include "EnvironmentEx.h"
#include <Nirvana/ImportInterface.h>

namespace CORBA {

class Any;

enum TCKind : Nirvana::ABI_enum
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

typedef Nirvana::I_out <TypeCode> TypeCode_out;
typedef Nirvana::I_inout <TypeCode> TypeCode_inout;

typedef Short Visibility;
const Visibility PRIVATE_MEMBER = 0;
const Visibility PUBLIC_MEMBER = 1;

typedef Short ValueModifier;
const ValueModifier VM_NONE = 0;
const ValueModifier VM_CUSTOM = 1;
const ValueModifier VM_ABSTRACT = 2;
const ValueModifier VM_TRUNCATABLE = 3;

namespace Nirvana {

class Marshal;
typedef I_ptr <Marshal> Marshal_ptr;

class Unmarshal;
typedef I_ptr <Unmarshal> Unmarshal_ptr;

BRIDGE_BEGIN (TypeCode, CORBA_REPOSITORY_ID ("TypeCode"))
Boolean (*equal) (Bridge <TypeCode>*, Interface*, EnvironmentBridge*);
Boolean (*equivalent) (Bridge <TypeCode>*, Interface*, EnvironmentBridge*);
Interface* (*get_compact_typecode) (Bridge <TypeCode>*, EnvironmentBridge*);
ABI_enum (*kind) (Bridge <TypeCode>*, EnvironmentBridge*);
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
ABI_boolean (*_has_marshal) (Bridge <TypeCode>*, EnvironmentBridge*);
void (*_marshal_in) (Bridge <TypeCode>*, ::Nirvana::ConstPointer, Interface*, ::Nirvana::Pointer, EnvironmentBridge*);
void (*_marshal_out) (Bridge <TypeCode>*, ::Nirvana::Pointer, Interface*, ::Nirvana::Pointer, EnvironmentBridge*);
void (*_unmarshal) (Bridge <TypeCode>*, ::Nirvana::Pointer, Interface*, ::Nirvana::Pointer, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, TypeCode> :
	public T
{
public:
	Boolean equal (TypeCode_ptr other);
	Boolean equivalent (TypeCode_ptr other);
	TypeCode_var get_compact_typecode ();
	TCKind kind ();

	// for tk_objref, tk_struct, tk_union, tk_enum, tk_alias,
	// tk_value, tk_value_box, tk_native, tk_abstract_interface
	// tk_local_interface and tk_except
	const char* id (); // raises (BadKind);
	const char* name (); // raises (BadKind);

	// for tk_struct, tk_union, tk_enum, tk_value,
	// and tk_except
	ULong member_count (); // raises (BadKind);
	const char* member_name (ULong index); // raises (BadKind, Bounds);
	TypeCode_var member_type (ULong index); // raises (BadKind, Bounds);

	// for tk_union
	const Any& member_label (ULong index); // raises (BadKind, Bounds);
	TypeCode_var discriminator_type (); // raises (BadKind);
	Long default_index (); // raises (BadKind);

	// for tk_string, tk_sequence, and tk_array
	ULong length (); // raises (BadKind);

	// for tk_sequence, tk_array, tk_value_box and tk_alias
	TypeCode_var content_type (); // raises (BadKind);

	// for tk_fixed
	UShort fixed_digits (); // raises (BadKind);
	Short fixed_scale (); // raises (BadKind);

	// for tk_value
	Visibility member_visibility (ULong index); // raises (BadKind, Bounds);
	ValueModifier type_modifier (); // raises (BadKind);
	TypeCode_var concrete_base_type (); // raises (BadKind);

	// Nirvana extensions
	// NOTE: For exceptions, all methods below are related to the exception data, not the exception itself.

	// Size of the object.
	ULong _size ();

	// Call the default constructor.
	void _construct (::Nirvana::Pointer p);

	// Destroy the object.
	void _destruct (::Nirvana::Pointer p);

	// Call copy constructor.
	void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src);

	// Call move constructor. Fallbacks to copy constructor if no move constructor exists.
	void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src);

	// If returns `false` then `marshaler` and `unmarshaler` parameters may be `nil`.
	Boolean _has_marshal ();

	void _marshal_in (::Nirvana::ConstPointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst);
	void _marshal_out (::Nirvana::Pointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst);
	void _unmarshal (::Nirvana::Pointer src, Unmarshal_ptr unmarshaler, ::Nirvana::Pointer dst);
};

}

class TypeCode : public ::CORBA::Nirvana::ClientInterface <TypeCode>
{
public:
	static const ::Nirvana::ImportInterfaceT <TypeCode> _tc_BadKind;

	class BadKind : public UserException
	{
	public:
		DECLARE_EXCEPTION (BadKind);
	};

	static const ::Nirvana::ImportInterfaceT <TypeCode> _tc_Bounds;

	class Bounds : public UserException
	{
	public:
		DECLARE_EXCEPTION (Bounds);
	};
};

extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_TCKind;

}

#endif
