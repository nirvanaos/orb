#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"
#include "InterfaceMetadata.h"
#include <array>
namespace CORBA {
namespace Nirvana {

class TypeCodeMembersEmpty
{
public:
	static ULong _member_count (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return 0;
	}

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		TypeCodeBase::set_Bounds (_env);
		return nullptr;
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		TypeCodeBase::set_Bounds (_env);
		return nullptr;
	}
};

template <ULong N>
class TypeCodeMemberCount
{
public:
	static const ULong member_count_ = N;

	static ULong _member_count (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return member_count_;
	}
};

template <class T>
class TypeCodeMembers
{
protected:
	static const Parameter members_ [];
};

template <class Base>
class TypeCodeNoMembers :
	public Base,
	public TypeCodeMembersEmpty
{
public:
	using TypeCodeMembersEmpty::_member_count;
	using TypeCodeMembersEmpty::_member_name;
	using TypeCodeMembersEmpty::_member_type;
};

template <class T, size_t N, class Base>
class TypeCodeWithMembers :
	public Base,
	public TypeCodeMemberCount <N>,
	public TypeCodeMembers <T>
{
public:
	using TypeCodeMemberCount <N>::_member_count;

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		if (index >= N) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else
			return TypeCodeMembers <T>::members_ [index].name;
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		if (index >= N) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else
			return &TypeCode::_duplicate (TypeCode_ptr (TypeCodeMembers <T>::members_ [index].type));
	}
};

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
