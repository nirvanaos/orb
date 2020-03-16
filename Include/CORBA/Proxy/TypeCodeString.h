#ifndef NIRVANA_ORB_TYPECODESTRING_H_
#define NIRVANA_ORB_TYPECODESTRING_H_

#include "TypeCodeImpl.h"
#include "../String.h"

namespace CORBA {
namespace Nirvana {

template <typename Valtype, TCKind TK, ULong bound = 0>
class TypeCodeString :
	public TypeCodeImpl <TypeCodeString <Valtype, TK, bound>, TK>,
	public TypeCodeOps <Valtype>,
	public TypeCodeLength <bound>
{
public:
	using TypeCodeLength <bound>::_length;
};

typedef TypeCodeString <String, tk_string> TC_string;
typedef TypeCodeString <WString, tk_wstring> TC_wstring;

}
}

#endif
