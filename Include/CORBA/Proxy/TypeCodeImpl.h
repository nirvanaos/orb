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

template <TCKind tk>
class TypeCodeTK :
	public TypeCodeBase
{
public:
	static const TCKind tk_ = tk;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (tk_, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (tk_, other);
	}

	static Type <TCKind>::ABI_ret _kind (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return tk_;
	}
};

template <TCKind tk, class T>
class TypeCodeWithId :
	public TypeCodeTK <tk>
{
public:
	typedef T RepositoryType;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (tk, RepositoryType::repository_id_, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (tk, RepositoryType::repository_id_, other);
	}

	static const char* _id (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return RepositoryType::repository_id_;
	}
};

template <typename T>
class TypeCodeOps
{
public:
	typedef T Valtype;

	static size_t __size (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
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

	static ABI_boolean __has_marshal (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return MarshalTraits <Valtype>::has_marshal;
	}

	static void _marshal_in (::Nirvana::ConstPointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <Valtype>::has_check)
			Type <Valtype>::check (*reinterpret_cast <const typename Type <Valtype>::ABI_type*> (src));
		MarshalTraits <Valtype>::marshal_in (*reinterpret_cast <const Valtype*> (src), marshaler, *reinterpret_cast <typename Type <Valtype>::ABI_type*> (dst));
	}

	static void _marshal_out (::Nirvana::Pointer src, Marshal_ptr marshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		if (Type <Valtype>::has_check)
			Type <Valtype>::check (*reinterpret_cast <const typename Type <Valtype>::ABI_type*> (src));
		MarshalTraits <Valtype>::marshal_out (*reinterpret_cast <Valtype*> (src), marshaler, *reinterpret_cast <typename Type <Valtype>::ABI_type*> (dst));
	}

	static void _unmarshal (::Nirvana::ConstPointer src, Unmarshal_ptr unmarshaler, ::Nirvana::Pointer dst)
	{
		_check_pointer (dst);
		_check_pointer (src);
		// Do not call check() here, unmarshal() will check.
		MarshalTraits <Valtype>::unmarshal (*reinterpret_cast <const typename Type <Valtype>::ABI_type*> (src), unmarshaler, *reinterpret_cast <Valtype*> (dst));
	}
};

template <>
class TypeCodeOps <void>
{
public:
	typedef void Valtype;

	static size_t __size (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
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

	static ABI_boolean __has_marshal (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return 0;
	}

	static void __marshal_in (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* marshaler, ::Nirvana::Pointer dst, EnvironmentBridge* _env)
	{}

	static void __marshal_out (Bridge <TypeCode>* _b, ::Nirvana::Pointer src, Interface* marshaler, ::Nirvana::Pointer dst, EnvironmentBridge* _env)
	{}

	static void __unmarshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, Interface* unmarshaler, ::Nirvana::Pointer, EnvironmentBridge* _env)
	{}
};

template <class S, class Impl, class Ops>
class TypeCodeImplBase :
	public S, public Impl, public Ops
{
public:
	using Impl::_kind;
	using Impl::_id;
	using Impl::_name;
	using Impl::_member_count;
	using Impl::_member_name;
	using Impl::_member_type;
	using Impl::_member_label;
	using Impl::_discriminator_type;
	using Impl::_default_index;
	using Impl::_length;
	using Impl::_content_type;
	using Impl::_fixed_digits;
	using Impl::_fixed_scale;
	using Impl::_member_visibility;
	using Impl::_type_modifier;
	using Impl::_concrete_base_type;

	using Ops::__size;
	using Ops::__has_marshal;

	// The get_compact_typecode operation strips out all optional name and member
	// name fields, but it leaves all alias typecodes intact.
	TypeCode_var get_compact_typecode ()
	{
		// Currently, we don't strip names, just return this type code.
		return TypeCode::_duplicate (&static_cast <TypeCode&> (static_cast <Bridge <TypeCode>&> (static_cast <S&> (*this))));
	}
};

template <class S, class Impl, class Ops>
class TypeCodeImpl :
	public TypeCodeImplBase <S, Impl, Ops>
{};

template <class S, class Impl>
class TypeCodeImpl <S, Impl, TypeCodeOps <void> > :
	public TypeCodeImplBase <S, Impl, TypeCodeOps <void> >
{
public:
	using TypeCodeOps <void>::__construct;
	using TypeCodeOps <void>::__destruct;
	using TypeCodeOps <void>::__copy;
	using TypeCodeOps <void>::__move;
	using TypeCodeOps <void>::__marshal_in;
	using TypeCodeOps <void>::__marshal_out;
	using TypeCodeOps <void>::__unmarshal;
};

template <class S, class Impl, class Ops>
class TypeCodeStatic :
	public TypeCodeImpl <ServantStatic <S, TypeCode>, Impl, Ops>
{};

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

// for tk_sequence, tk_array, tk_value_box, tk_alias
template <const ::Nirvana::ImportInterfaceT <TypeCode>* ptc>
class TypeCodeContentType
{
public:
	static Interface* _content_type (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		TypeCode_ptr tc (*ptc);
		return (tc->_epv ().header.duplicate) (&tc, _env);
	}
};

// for tk_struct, tk_union, tk_enum, tk_value, tk_except
template <ULong member_count>
class TypeCodeMemberCount
{
public:
	static const ULong member_count_ = member_count;

	static ULong _member_count (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return member_count_;
	}
};

}
}

#endif
