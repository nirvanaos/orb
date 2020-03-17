#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Nirvana {

template <class E> class TypeCodeException;

template <class E>
class TypeCodeExceptionBase :
	public TypeCodeNoMembers <TypeCodeWithId <tk_except, E::repository_id_> >
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return E::__name ();
	}
};

template <class E>
class TypeCodeExceptionSystem :
	public TypeCodeStatic <TypeCodeException <E>, TypeCodeExceptionBase <E>, TypeCodeOps <SystemException::Data> >
{};

template <class E>
class TypeCodeExceptionEmpty :
	public TypeCodeStatic <TypeCodeException <E>, TypeCodeExceptionBase <E>, TypeCodeOpsEmpty>
{};

template <class E, size_t N>
class TypeCodeExceptionWithData :
	public TypeCodeStatic <TypeCodeException <E>,
	TypeCodeWithMembers <TypeCodeException <E>, N, TypeCodeExceptionBase <E> >, TypeCodeOps <typename E::Data> >
{};

}
}

#endif
