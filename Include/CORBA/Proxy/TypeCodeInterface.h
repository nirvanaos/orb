#ifndef NIRVANA_ORB_TYPECODEINTERFACE_H_
#define NIRVANA_ORB_TYPECODEINTERFACE_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

template <class I>
class TypeCodeInterface :
	public TypeCodeWithId <TypeCodeInterface <I>, tk_objref, I::interface_id_>,
	public TypeCodeOps <I_var <I> >
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return name_;
	}

	static const Char name_ [];
};

}
}

#define DEFINE_INTERFACE_TC(ns, I) template <> const Char TypeCodeInterface <ns::I>::name_ [] = #I;

#endif
