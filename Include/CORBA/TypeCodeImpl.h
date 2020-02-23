#ifndef NIRVANA_ORB_TYPECODEIMPL_H_
#define NIRVANA_ORB_TYPECODEIMPL_H_

#include "TypeCode_s.h"
#include "Exception.h"
#include <new>

namespace CORBA {
namespace Nirvana {

template <class T> struct MarshalTraits;

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
		new (reinterpret_cast <Valtype*> (p)) Valtype ();
	}

	static void _destruct (::Nirvana::Pointer p)
	{
		reinterpret_cast <Valtype*> (p)->~Valtype ();
	}

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		new (dst) Valtype (*reinterpret_cast <const Valtype*> (src));
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		new (dst) Valtype (std::move (*reinterpret_cast <Valtype*> (src)));
	}

	static void _local_marshal (::Nirvana::ConstPointer src, ::Nirvana::Pointer dst)
	{
		MarshalTraits <Valtype>::local_marshal (*reinterpret_cast <const Valtype*> (src), *reinterpret_cast <Valtype*> (dst));
	}

	static void _local_unmarshal_in (::Nirvana::Pointer val)
	{
		MarshalTraits <Valtype>::local_unmarshal_in (*reinterpret_cast <Valtype*> (val));
	}

	static void _local_unmarshal_inout (::Nirvana::Pointer val)
	{
		MarshalTraits <Valtype>::local_unmarshal_inout (*reinterpret_cast <Valtype*> (val));
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

	static void __local_marshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, ::Nirvana::Pointer dst, EnvironmentBridge* _env)
	{}

	static void __local_unmarshal_in (Bridge <TypeCode>* _b, ::Nirvana::Pointer val, EnvironmentBridge* _env)
	{}

	static void __local_unmarshal_inout (Bridge <TypeCode>* _b, ::Nirvana::Pointer val, EnvironmentBridge* _env)
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
	using TypeCodeOpsEmptyBase::__local_marshal;
	using TypeCodeOpsEmptyBase::__local_unmarshal_in;
	using TypeCodeOpsEmptyBase::__local_unmarshal_inout;
};

}
}

#endif
