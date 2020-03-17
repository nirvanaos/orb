#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"
#include "TypeCodeMembers.h"

namespace CORBA {
namespace Nirvana {

template <class E> class TypeCodeException;

template <class E>
class TypeCodeExceptionBase :
	public TypeCodeNoMembers <TypeCodeWithId <TypeCodeException <E>, tk_except, E::repository_id_> >
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return E::__name ();
	}
};

template <class E>
class TypeCodeExceptionSystem :
	public TypeCodeExceptionBase <E>,
	public TypeCodeOps <SystemException::Data>
{};

template <class E>
class TypeCodeExceptionEmpty :
	public TypeCodeExceptionBase <E>,
	public TypeCodeOpsEmpty
{};

template <class E, size_t N>
class TypeCodeExceptionWithData :
	public TypeCodeWithMembers <TypeCodeException <E>, N, TypeCodeExceptionBase <E>>,
	public TypeCodeOps <typename E::Data>
{};

}
}

#define IMPLEMENT_EXCEPTION_TC1(ns, E) namespace ns { typedef TypeCodeException <E> TC_##E; }\
NIRVANA_EXPORT_INTERFACE1 (ns, ns::E::repository_id_, TC_##E, CORBA::Nirvana::TypeCode)

#endif
