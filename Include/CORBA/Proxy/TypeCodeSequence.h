#ifndef NIRVANA_ORB_TYPECODESEQUENCE_H_
#define NIRVANA_ORB_TYPECODESEQUENCE_H_

#include "TypeCodeImpl.h"
#include "../Sequence.h"

namespace CORBA {
namespace Nirvana {

template <typename T, const ::Nirvana::ImportInterfaceT <TypeCode>* ptc, ULong bound = 0>
class TypeCodeSequence :
	public TypeCodeStatic <TypeCodeSequence <T, ptc, bound>, TypeCodeTK <tk_sequence>, TypeCodeOps <Sequence <T> > >,
	public TypeCodeLength <bound>,
	public TypeCodeContentType <ptc>
{
public:
	using TypeCodeLength <bound>::_length;
	using TypeCodeContentType <ptc>::_content_type;

	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeTK <tk_sequence>::equal (other)
			&& other->length () == bound
			&& other->content_type ()->equal (*ptc);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeTK <tk_sequence>::equivalent (other)
			&& other->length () == bound
			&& other->content_type ()->equivalent (*ptc);
	}
};

}
}

#endif
