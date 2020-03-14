#ifndef NIRVANA_ORB_TYPECODE_H_
#define NIRVANA_ORB_TYPECODE_H_

#include "TypeCode_b.h"
#include "Client.h"
#include "Type_interface.h"
#include "UserException.h"
#include "EnvironmentEx.h"
#include <Nirvana/ImportInterface.h>

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

template <> struct Type <TCKind> : TypeEnum <TCKind>
{
	static void check (TypeEnum <TCKind>::ABI_type tk)
	{
		if (tk > tk_local_interface)
			::Nirvana::throw_BAD_PARAM ();
	}
};

class PlatformMarshal;
typedef I_ptr <PlatformMarshal> PlatformMarshal_ptr;

class PlatformUnmarshal;
typedef I_ptr <PlatformUnmarshal> PlatformUnmarshal_ptr;

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

	// Size of object.
	ULong _size ();

	// Call default constructor.
	void _construct (::Nirvana::Pointer p);

	// Destroy object.
	void _destruct (::Nirvana::Pointer p);

	// Call copy constructor.
	void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src);

	// Call move constructor. Fallbacks to copy constructor if no move constructor exists.
	void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src);

	void _marshal_in (::Nirvana::ConstPointer src, PlatformMarshal_ptr marshaler, ::Nirvana::Pointer dst);
	void _marshal_out (::Nirvana::Pointer src, PlatformMarshal_ptr marshaler, ::Nirvana::Pointer dst);
	void _unmarshal (::Nirvana::Pointer src, PlatformUnmarshal_ptr marshaler, ::Nirvana::Pointer dst);
};

}

class TypeCode : public ::CORBA::Nirvana::ClientInterface <TypeCode>
{
public:
	class BadKind : public UserException
	{
	public:
		DECLARE_EXCEPTION (BadKind);
	};

	static const ::Nirvana::ImportInterfaceT <TypeCode> _tc_BadKind;

	class Bounds : public UserException
	{
	public:
		DECLARE_EXCEPTION (Bounds);
	};

	static const ::Nirvana::ImportInterfaceT <TypeCode> _tc_Bounds;
};

namespace Nirvana {

template <class T>
Boolean Client <T, TypeCode>::equal (TypeCode_ptr other)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.equal) (&_b, &other, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, TypeCode>::equivalent (TypeCode_ptr other)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.equivalent) (&_b, &other, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::get_compact_typecode ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.get_compact_typecode) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TCKind Client <T, TypeCode>::kind ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Type <TCKind>::C_ret _ret = (_b._epv ().epv.kind) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
const char* Client <T, TypeCode>::id ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	const char* _ret = (_b._epv ().epv.id) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
const char* Client <T, TypeCode>::name ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	const char* _ret = (_b._epv ().epv.name) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, TypeCode>::member_count ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.member_count) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
const char* Client <T, TypeCode>::member_name (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	const char* _ret = (_b._epv ().epv.member_name) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::member_type (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.member_type) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
const Any& Client <T, TypeCode>::member_label (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	typename Type <Any>::C_VT_ret _ret = (_b._epv ().epv.member_label) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::discriminator_type ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.discriminator_type) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Long Client <T, TypeCode>::default_index ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.default_index) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, TypeCode>::length ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.length) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::content_type ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.content_type) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
UShort Client <T, TypeCode>::fixed_digits ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	UShort _ret = (_b._epv ().epv.fixed_digits) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Short Client <T, TypeCode>::fixed_scale ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Short _ret = (_b._epv ().epv.fixed_scale) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Visibility Client <T, TypeCode>::member_visibility (ULong index)
{
	EnvironmentEx <TypeCode::BadKind, TypeCode::Bounds> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	Visibility _ret = (_b._epv ().epv.member_visibility) (&_b, index, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ValueModifier Client <T, TypeCode>::type_modifier ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ValueModifier _ret = (_b._epv ().epv.type_modifier) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, TypeCode>::concrete_base_type ()
{
	EnvironmentEx <TypeCode::BadKind> _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.concrete_base_type) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, TypeCode>::_size ()
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv._size) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, TypeCode>::_construct (::Nirvana::Pointer p)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._construct) (&_b, p, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_destruct (::Nirvana::Pointer p)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._destruct) (&_b, p, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._copy) (&_b, dst, src, &_env);
	_env.check ();
}

template <class T>
void Client <T, TypeCode>::_move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
{
	Environment _env;
	Bridge <TypeCode>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._move) (&_b, dst, src, &_env);
	_env.check ();
}

}
}

#endif
