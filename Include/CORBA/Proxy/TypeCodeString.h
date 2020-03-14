#ifndef NIRVANA_ORB_TYPECODESTRING_H_
#define NIRVANA_ORB_TYPECODESTRING_H_

#include "TypeCodeImpl.h"
#include <Nirvana/basic_string.h>

namespace CORBA {
namespace Nirvana {

class TypeCodeStringBase
{
public:
	static ULong _length (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return 0;
	}
};

template <typename Valtype, TCKind TK>
class TypeCodeString :
	public TypeCodeImpl <TypeCodeString <Valtype, TK>, TK>,
	public TypeCodeOps <Valtype>,
	public TypeCodeStringBase
{
public:
	using TypeCodeStringBase::_length;
};

typedef TypeCodeString <std::basic_string <Char>, tk_string> TC_string;
typedef TypeCodeString <std::basic_string <WChar>, tk_wstring> TC_wstring;

}
}

#endif
