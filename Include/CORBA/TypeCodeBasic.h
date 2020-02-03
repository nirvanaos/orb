#ifndef NIRVANA_ORB_TYPECODEBASIC_H_
#define NIRVANA_ORB_TYPECODEBASIC_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <typename S, TCKind TK>
class TypeCodeBasicBase :
	public TypeCodeBase,
	public ServantStatic <S, TypeCode>
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

	static BridgeMarshal <TypeCode>* _get_compact_typecode (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return InterfaceStatic <S, TypeCode>::_bridge ();
	}

	static TCKind _kind (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return TK;
	}
};

/// TypeCode implementation for basic types
template <typename T, TCKind TK>
class TypeCodeBasic :
	public TypeCodeBasicBase <TypeCodeBasic <T, TK>, TK>,
	public TypeCodeOps <T>
{};

/// TypeCode implementation for empty types
template <TCKind TK>
class TypeCodeEmpty :
	public TypeCodeBasicBase <TypeCodeEmpty <TK>, TK>
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
};

typedef TypeCodeEmpty <tk_null> TC_null;
typedef TypeCodeEmpty <tk_void> TC_void;
typedef TypeCodeBasic <Short, tk_short> TC_short;
typedef TypeCodeBasic <Long, tk_long> TC_long;
typedef TypeCodeBasic <UShort, tk_ushort> TC_ushort;
typedef TypeCodeBasic <ULong, tk_ulong> TC_ulong;
typedef TypeCodeBasic <Float, tk_float> TC_float;
typedef TypeCodeBasic <Double, tk_double> TC_double;
typedef TypeCodeBasic <Boolean, tk_boolean> TC_boolean;
typedef TypeCodeBasic <Char, tk_char> TC_char;
typedef TypeCodeBasic <Octet, tk_octet> TC_octet;
typedef TypeCodeBasic <Any, tk_any> TC_any;
typedef TypeCodeBasic <TypeCode_ptr, tk_TypeCode> TC_TypeCode;
typedef TypeCodeBasic <LongLong, tk_longlong> TC_longlong;
typedef TypeCodeBasic <ULongLong, tk_ulonglong> TC_ulonglong;
typedef TypeCodeBasic <LongDouble, tk_longdouble> TC_longdouble;
typedef TypeCodeBasic <WChar, tk_wchar> TC_wchar;

}
}

#endif
