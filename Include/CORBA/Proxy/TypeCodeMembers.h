#ifndef NIRVANA_ORB_TYPECODEMEMBERS_H_
#define NIRVANA_ORB_TYPECODEMEMBERS_H_

#include "InterfaceMetadata.h"

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

}
}

#endif
