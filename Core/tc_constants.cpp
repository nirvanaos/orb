#include <CORBA/CORBA.h>
#include <CORBA/Proxy/TypeCodeString.h>
#include "tc_impex.h"

namespace CORBA {

namespace Nirvana {

template <typename T, TCKind tk>
class TypeCodeScalar : public TypeCodeStatic <TypeCodeScalar <T, tk>, TypeCodeTK <tk>, TypeCodeOps <T> > {};

}

class TC_TypeCode :
	public Nirvana::TypeCodeStatic <TC_TypeCode, Nirvana::TypeCodeTK <tk_TypeCode>, Nirvana::TypeCodeOps <TypeCode_var> >
{};

class TC_Object :
	public Nirvana::TypeCodeStatic <TC_Object, Nirvana::TypeCodeWithId <tk_objref, Object>, Nirvana::TypeCodeOps <Object_var> >
{
public:
	static const char* _name (Nirvana::Bridge <TypeCode>* _b, Nirvana::EnvironmentBridge* _env)
	{
		return "Object";
	}
};

typedef Nirvana::TypeCodeString <Nirvana::String, 0> TC_string;
typedef Nirvana::TypeCodeString <Nirvana::WString, 0> TC_wstring;

}
/*
namespace CORBA {
typedef Nirvana::TypeCodeScalar <void, tk_void> TC_void;
extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_void{ 0, nullptr, nullptr, STATIC_BRIDGE (TC_void, TypeCode) };
} NIRVANA_EXPORT_INTERFACE1 (CORBA, "CORBA/_tc_void", TC_void, CORBA::TypeCode);
*/

#define TC_IMPL_SCALAR(T, t) namespace CORBA {typedef Nirvana::TypeCodeScalar <T, tk_##t> TC_##t;} TC_IMPEX(t)

TC_IMPL_SCALAR (void, void)
TC_IMPL_SCALAR (Short, short)
TC_IMPL_SCALAR (Long, long)
TC_IMPL_SCALAR (UShort, ushort)
TC_IMPL_SCALAR (ULong, ulong)
TC_IMPL_SCALAR (Float, float)
TC_IMPL_SCALAR (Double, double)
TC_IMPL_SCALAR (Boolean, boolean)
TC_IMPL_SCALAR (Char, char)
TC_IMPL_SCALAR (Octet, octet)
TC_IMPL_SCALAR (LongLong, longlong)
TC_IMPL_SCALAR (ULongLong, ulonglong)
TC_IMPL_SCALAR (LongDouble, longdouble)
TC_IMPL_SCALAR (WChar, wchar)
TC_IMPL_SCALAR (Any, any)
TC_IMPEX (TypeCode)
TC_IMPEX_BY_ID (Object)
TC_IMPEX (string)
TC_IMPEX (wstring)
//TC_IMPEX (ValueBase)

