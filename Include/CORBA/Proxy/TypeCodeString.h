#ifndef NIRVANA_ORB_TYPECODESTRING_H_
#define NIRVANA_ORB_TYPECODESTRING_H_

#include "TypeCodeImpl.h"
#include "../String.h"

namespace CORBA {
namespace Nirvana {

template <typename Valtype, ULong bound> class TypeCodeString;

template <typename Valtype, TCKind tk, ULong bound>
class TypeCodeStringBase :
	public TypeCodeStatic <TypeCodeString <Valtype, bound>, TypeCodeTK <tk>, TypeCodeOps <Valtype> >,
	public TypeCodeLength <bound>
{
public:
	using TypeCodeLength <bound>::_length;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeTK <tk>::equal (other)
			&& other->length () == bound;
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeTK <tk>::equivalent (other)
			&& other->length () == bound;
	}
};

template <ULong bound>
class TypeCodeString <String, bound> : public TypeCodeStringBase <String, tk_string, bound> {};

template <ULong bound>
class TypeCodeString <WString, bound> : public TypeCodeStringBase <WString, tk_wstring, bound> {};

}
}

#endif
