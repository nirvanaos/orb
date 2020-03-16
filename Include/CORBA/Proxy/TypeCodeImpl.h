#ifndef NIRVANA_ORB_TYPECODEIMPL_H_
#define NIRVANA_ORB_TYPECODEIMPL_H_

#include "../TypeCode_s.h"
#include "../Exception.h"
#include "MarshalTraits.h"
#include <new>

namespace CORBA {
namespace Nirvana {

class TypeCodeBase
{
public:
	static Boolean equal (TCKind tk, TypeCode_ptr other);
	static Boolean equivalent (TCKind tk, TypeCode_ptr other);
	static Boolean equal (TCKind tk, const char* id, TypeCode_ptr other);
	static Boolean equivalent (TCKind tk, const char* id, TypeCode_ptr other);

	static const char* _id (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static ULong _member_count (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env);
	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env);
	static const Any* _member_label (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env);
	static Interface* _discriminator_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static Long _default_index (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static ULong _length (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static Interface* _content_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static UShort _fixed_digits (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static Short _fixed_scale (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static Visibility _member_visibility (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env);
	static ValueModifier _type_modifier (Bridge <TypeCode>* _b, EnvironmentBridge* _env);
	static Interface* _concrete_base_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env);

	static void set_BadKind (EnvironmentBridge* env);
	static void set_Bounds (EnvironmentBridge* env);
};

template <class S, TCKind tk>
class TypeCodeImpl :
	public TypeCodeBase,
	public ServantStatic <S, TypeCode>
{
public:
	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (tk, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (tk, other);
	}

	static Interface* _get_compact_typecode (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return InterfaceStatic <S, TypeCode>::_bridge ();
	}

	static Type <TCKind>::ABI_ret _kind (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return tk;
	}

	using TypeCodeBase::_id;
	using TypeCodeBase::_name;
	using TypeCodeBase::_member_count;
	using TypeCodeBase::_member_name;
	using TypeCodeBase::_member_type;
	using TypeCodeBase::_member_label;
	using TypeCodeBase::_discriminator_type;
	using TypeCodeBase::_default_index;
	using TypeCodeBase::_length;
	using TypeCodeBase::_content_type;
	using TypeCodeBase::_fixed_digits;
	using TypeCodeBase::_fixed_scale;
	using TypeCodeBase::_member_visibility;
	using TypeCodeBase::_type_modifier;
	using TypeCodeBase::_concrete_base_type;
};

template <class S, TCKind tk, const char* rep_id>
class TypeCodeWithId :
	public TypeCodeImpl <S, tk>
{
public:
	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (tk, rep_id, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (tk, rep_id, other);
	}

	static const char* _id (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return rep_id;
	}
};

template <typename Valtype>
class TypeCodeOps
{
public:
	static ULong _size ()
	{
		return sizeof (Valtype);
	}

	static void _construct (::Nirvana::Pointer p)
	{
		_check_pointer (p);
		new (p) Valtype ();
	}

	static void _destruct (::Nirvana::Pointer p)
	{
		if (p)
			reinterpret_cast <Valtype*> (p)->~Valtype ();
	}

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <Valtype>::has_check)
			Type <Valtype>::check (*reinterpret_cast <const typename Type <Valtype>::ABI_type*> (src));
		new (dst) Valtype (*reinterpret_cast <const Valtype*> (src));
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <Valtype>::has_check)
			Type <Valtype>::check (*reinterpret_cast <const typename Type <Valtype>::ABI_type*> (src));
		new (dst) Valtype (std::move (*reinterpret_cast <Valtype*> (src)));
	}

	static void _marshal_in (::Nirvana::ConstPointer src, PlatformMarshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <Valtype>::has_check)
			Type <Valtype>::check (*reinterpret_cast <const typename Type <Valtype>::ABI_type*> (src));
		MarshalTraits <Valtype>::marshal_in (*reinterpret_cast <const Valtype*> (src), marshaler, *reinterpret_cast <typename Type <Valtype>::ABI_type*> (dst));
	}

	static void _marshal_out (::Nirvana::Pointer src, PlatformMarshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <Valtype>::has_check)
			Type <Valtype>::check (*reinterpret_cast <const typename Type <Valtype>::ABI_type*> (src));
		MarshalTraits <Valtype>::marshal_out (*reinterpret_cast <Valtype*> (src), marshaler, *reinterpret_cast <typename Type <Valtype>::ABI_type*> (dst));
	}

	static void _unmarshal (::Nirvana::Pointer src, PlatformUnmarshal_ptr unmarshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		// Do not call check() here, unmarshal() will check.
		MarshalTraits <Valtype>::unmarshal (*reinterpret_cast <typename Type <Valtype>::ABI_type*> (src), unmarshaler, *reinterpret_cast <Valtype*> (dst));
	}
};

class TypeCodeOpsEmptyBase
{
public:
	static ULong __size (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return 0;
	}

	static void __construct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, EnvironmentBridge* _env)
	{}

	static void __destruct (Bridge <TypeCode>* _b, ::Nirvana::Pointer p, EnvironmentBridge* _env)
	{}

	static void __copy (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::ConstPointer src, EnvironmentBridge* _env)
	{}

	static void __move (Bridge <TypeCode>* _b, ::Nirvana::Pointer dst, ::Nirvana::Pointer src, EnvironmentBridge* _env)
	{}

	static void __marshal_in (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* marshaler, ::Nirvana::Pointer dst, EnvironmentBridge* _env)
	{}

	static void __marshal_out (Bridge <TypeCode>* _b, ::Nirvana::Pointer src, Interface* marshaler, ::Nirvana::Pointer dst, EnvironmentBridge* _env)
	{}

	static void __unmarshal (Bridge <TypeCode>* _b, ::Nirvana::Pointer src, Interface* unmarshaler, ::Nirvana::Pointer, EnvironmentBridge* _env)
	{}
};

template <class Base>
class TypeCodeOpsEmpty : 
	public Base,
	public TypeCodeOpsEmptyBase
{
public:
	using TypeCodeOpsEmptyBase::__size;
	using TypeCodeOpsEmptyBase::__construct;
	using TypeCodeOpsEmptyBase::__destruct;
	using TypeCodeOpsEmptyBase::__copy;
	using TypeCodeOpsEmptyBase::__move;
	using TypeCodeOpsEmptyBase::__marshal_in;
	using TypeCodeOpsEmptyBase::__marshal_out;
	using TypeCodeOpsEmptyBase::__unmarshal;
};

// for tk_string, tk_sequence, and tk_array
template <ULong bound = 0>
class TypeCodeLength
{
public:
	static ULong _length (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return bound;
	}
};

// for tk_sequence, tk_array, tk_value_box and tk_alias
template <const ::Nirvana::ImportInterfaceT <TypeCode>* ptc>
class TypeCodeContentType
{
public:
	static Interface* _content_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return &TypeCode_ptr (*ptc);
	}
};

}
}

#endif
