#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

class TypeCodeExceptionBase
{
public:
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

template <class S, class Ex>
class TypeCodeExceptionVoid :
	public TypeCodeOpsEmpty <TypeCodeWithId <S, tk_except, Ex::repository_id_> >
{};

template <class S, class Ex>
class TypeCodeExceptionData :
	public TypeCodeWithId <S, tk_except, Ex::repository_id_>,
	public TypeCodeOps <typename Ex::Data>
{};

template <class Ex>
class TypeCodeException :
	public std::conditional <std::is_void <typename Ex::Data>::value,
		TypeCodeExceptionVoid <TypeCodeException <Ex>, Ex>,
		TypeCodeExceptionData <TypeCodeException <Ex>, Ex> >::type,
	public TypeCodeExceptionBase
{
public:
	static const char* _name (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return Ex::__name ();
	}

	static ULong _member_count (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return 0;
	}

	using TypeCodeExceptionBase::_member_name;
	using TypeCodeExceptionBase::_member_type;
};

}
}

#endif
