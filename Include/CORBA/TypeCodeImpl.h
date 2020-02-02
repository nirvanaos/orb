#ifndef NIRVANA_ORB_TYPECODEIMPL_H_
#define NIRVANA_ORB_TYPECODEIMPL_H_

#include "TypeCode_s.h"

namespace CORBA {
namespace Nirvana {

class TypeCodeBase
{
public:
	static const char* id ()
	{
		throw_bad_kind ();
	}

	static const char* name ()
	{
		throw_bad_kind ();
	}

	static ULong member_count ()
	{
		throw_bad_kind ();
	}

	static Identifier member_name (ULong index)
	{
		throw_bad_kind ();
	}

	static TypeCode_ptr member_type (ULong index)
	{
		throw_bad_kind ();
	}

	static Any* member_label (ULong index)
	{
		throw_bad_kind ();
	}

	static TypeCode_ptr discriminator_type ()
	{
		throw_bad_kind ();
	}

	static Long default_index ()
	{
		throw_bad_kind ();
	}

	static ULong length ()
	{
		throw_bad_kind ();
	}

	static TypeCode_ptr content_type ()
	{
		throw_bad_kind ();
	}

	static UShort fixed_digits ()
	{
		throw_bad_kind ();
	}

	static Short fixed_scale ()
	{
		throw_bad_kind ();
	}

	static Visibility member_visibility (ULong index)
	{
		throw_bad_kind ();
	}

	static ValueModifier type_modifier ()
	{
		throw_bad_kind ();
	}

	static TypeCode_ptr concrete_base_type ()
	{
		throw_bad_kind ();
	}

protected:
	NIRVANA_NORETURN static void throw_bad_kind ();
	NIRVANA_NORETURN static void throw_bounds ();

	static Boolean equal (TCKind tk, TypeCode_ptr other);
	static Boolean equivalent (TCKind tk, TypeCode_ptr other);
};

template <typename T>
class TypeCodeOps
{
public:
	static ULong _size ()
	{
		return sizeof (T);
	}

	static void _construct (::Nirvana::Pointer p)
	{
		*reinterpret_cast <T*> (p) = 0;
	}

	static void _destruct (::Nirvana::Pointer p)
	{
		reinterpret_cast <T*> (p)->~T ();
	}

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		*reinterpret_cast <T*> (dst) = *reinterpret_cast <const T*> (src);
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		*reinterpret_cast <T*> (dst) = std::move (*reinterpret_cast <T*> (src));
	}
};

/// Template to get data type from TCKind
template <TCKind TK> struct TK_Type;

template <>
struct TK_Type <tk_short>
{
	typedef Short Type;
};

template <>
struct TK_Type <tk_long>
{
	typedef Long Type;
};

template <>
struct TK_Type <tk_ushort>
{
	typedef UShort Type;
};

template <>
struct TK_Type <tk_ulong>
{
	typedef ULong Type;
};

template <>
struct TK_Type <tk_float>
{
	typedef Float Type;
};

template <>
struct TK_Type <tk_double>
{
	typedef Double Type;
};

template <>
struct TK_Type <tk_boolean>
{
	typedef Boolean Type;
};

template <>
struct TK_Type <tk_char>
{
	typedef Char Type;
};

template <>
struct TK_Type <tk_octet>
{
	typedef Octet Type;
};

template <>
struct TK_Type <tk_any>
{
	typedef Any Type;
};

template <>
struct TK_Type <tk_TypeCode>
{
	typedef TypeCode_ptr Type;
};

template <>
struct TK_Type <tk_longlong>
{
	typedef LongLong Type;
};

template <>
struct TK_Type <tk_ulonglong>
{
	typedef ULongLong Type;
};

template <>
struct TK_Type <tk_longdouble>
{
	typedef LongDouble Type;
};

template <>
struct TK_Type <tk_wchar>
{
	typedef WChar Type;
};

/// TypeCode implementation for basic types
template <TCKind TK>
class TypeCodeBasicType :
	public ServantStatic <TypeCodeBasicType <TK>, TypeCode>,
	public TypeCodeBase,
	public TypeCodeOps <typename TK_Type <TK>::Type>
{
public:
	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (TK, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (TK, other);
	}

	static TypeCode_ptr get_compact_typecode ()
	{
		return ServantStatic <TypeCodeBasicType <TK>, TypeCode>::_get_ptr ();
	}

	static TCKind kind ()
	{
		return TK;
	}
};

/*
class TypeCodeException :
	public TypeCodeBase
{
public:
	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeBase::equal (tk, other);
	}

	static Boolean equivalent (TCKind tk, TypeCode_ptr other)
	{
		return TypeCodeBase::equivalent (tk, other);
	}

	const char* id (); // raises (BadKind);
	const char* name (); // raises (BadKind);
	ULong member_count (); // raises (BadKind);
	Identifier member_name (ULong index); // raises (BadKind, Bounds);
	TypeCode_ptr member_type (ULong index); // raises (BadKind, Bounds);
};
*/
}
}

#endif
