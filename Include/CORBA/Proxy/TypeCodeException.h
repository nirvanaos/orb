#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Nirvana {

template <class E> class TypeCodeException;

template <class E>
class TypeCodeExceptionRoot :
	public TypeCodeStatic <TypeCodeException <E>, TypeCodeWithId <tk_except, E::repository_id_>, TypeCodeOps <typename E::Data> >
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return E::__name ();
	}
};

class SystemExceptionMembers
{
protected:
	static const Parameter members_ [];
};

template <class E>
class TypeCodeExceptionSystem :
	public TypeCodeWithMembersImpl <2, TypeCodeExceptionRoot <E>, SystemExceptionMembers>
{};

template <class E, ULong member_count>
class TypeCodeExceptionImpl :
	public TypeCodeWithMembers <TypeCodeException <E>, member_count, TypeCodeExceptionRoot <E> >
{};

}
}

#endif
