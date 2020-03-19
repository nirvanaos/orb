#ifndef NIRVANA_ORB_TYPECODEMEMBERS_H_
#define NIRVANA_ORB_TYPECODEMEMBERS_H_

#include "TypeCodeImpl.h"
#include "InterfaceMetadata.h"

namespace CORBA {
namespace Nirvana {

template <class T>
class TypeCodeMembers
{
protected:
	static const Parameter members_ [];
};

template <ULong member_count, class Base, class Members>
class TypeCodeWithMembersImpl :
	public Base,
	public TypeCodeMemberCount <member_count>,
	public Members
{
public:
	using TypeCodeMemberCount <member_count>::_member_count;

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		if (index >= member_count) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else
			return Members::members_ [index].name;
	}

	static Interface* _member_type (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		if (index >= member_count) {
			TypeCodeBase::set_Bounds (_env);
			return nullptr;
		} else {
			TypeCode_ptr tc (Members::members_ [index].type);
			return (tc->_epv ().header.duplicate) (&tc, _env);
		}
	}
};

template <class T, ULong member_count, class Base>
class TypeCodeWithMembers :
	public TypeCodeWithMembersImpl <member_count, Base, TypeCodeMembers <T> >
{};

template <class T, class Base>
class TypeCodeWithMembers <T, 0, Base> :
	public Base,
	public TypeCodeMemberCount <0>
{
public:
	using TypeCodeMemberCount <0>::_member_count;

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

}
}

#endif
