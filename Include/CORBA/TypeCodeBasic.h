#ifndef NIRVANA_ORB_TYPECODEBASIC_H_
#define NIRVANA_ORB_TYPECODEBASIC_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <class S, TCKind TK>
class TypeCodeBasicBase :
	public TypeCodeImpl <S>
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

	static TCKind _kind (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return TK;
	}
};

/// TypeCode implementation for basic types
template <typename Valtype, TCKind TK>
class TypeCodeBasic :
	public TypeCodeBasicBase <TypeCodeBasic <Valtype, TK>, TK>,
	public TypeCodeOps <Valtype>
{};

/// TypeCode implementation for empty types
template <TCKind TK>
class TypeCodeEmpty :
	public TypeCodeOpsEmpty <TypeCodeBasicBase <TypeCodeEmpty <TK>, TK> >
{};

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
typedef TypeCodeBasic <TypeCode_var, tk_TypeCode> TC_TypeCode;
typedef TypeCodeBasic <LongLong, tk_longlong> TC_longlong;
typedef TypeCodeBasic <ULongLong, tk_ulonglong> TC_ulonglong;
typedef TypeCodeBasic <LongDouble, tk_longdouble> TC_longdouble;
typedef TypeCodeBasic <WChar, tk_wchar> TC_wchar;

}
}

#endif
