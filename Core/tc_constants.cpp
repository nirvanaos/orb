#include <CORBA/CORBA.h>
#include <CORBA/Proxy/TypeCodeString.h>
#include "tc_impex.h"

namespace CORBA {

namespace Nirvana {

template <typename T, TCKind tk>
class TypeCodeScalar : public TypeCodeStatic <TypeCodeScalar <T, tk>, TypeCodeTK <tk>, TypeCodeOps <T> > {};

}

using namespace Nirvana;

class TC_TypeCode :
	public TypeCodeStatic <TC_TypeCode, TypeCodeTK <tk_TypeCode>, TypeCodeOps <TypeCode_var> >
{};

class TC_Object :
	public TypeCodeStatic <TC_Object, TypeCodeWithId <tk_objref, Object>, TypeCodeOps <Object_var> >
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return "Object";
	}
};

typedef TypeCodeString <String, 0> TC_string;
typedef TypeCodeString <WString, 0> TC_wstring;

}

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

