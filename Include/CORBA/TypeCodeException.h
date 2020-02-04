#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

class TypeCodeExceptionBase
{
public:
	static Boolean equal (const char* id, TypeCode_ptr other);
	static Boolean equivalent (const char* id, TypeCode_ptr other);

	static const char* _member_name (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		TypeCodeBase::set_Bounds (_env);
		return nullptr;
	}

	static BridgeMarshal <TypeCode>* _member_type (Bridge <TypeCode>* _b, ULong index, EnvironmentBridge* _env)
	{
		TypeCodeBase::set_Bounds (_env);
		return nullptr;
	}

	static void __local_marshal (Bridge <TypeCode>* _b, ::Nirvana::ConstPointer src, ::Nirvana::Pointer dst, EnvironmentBridge* _env)
	{
		TypeCodeBase::set_BAD_TYPECODE (_env);
	}

	static void __local_unmarshal_in (Bridge <TypeCode>* _b, ::Nirvana::Pointer val, EnvironmentBridge* _env)
	{
		TypeCodeBase::set_BAD_TYPECODE (_env);
	}

	static void __local_unmarshal_inout (Bridge <TypeCode>* _b, ::Nirvana::Pointer val, EnvironmentBridge* _env)
	{
		TypeCodeBase::set_BAD_TYPECODE (_env);
	}
};

template <class Ex>
class TypeCodeException :
	public TypeCodeExceptionBase,
	public TypeCodeOps <Ex>,
	public TypeCodeImpl <TypeCodeException <Ex> >
{
public:
	static Boolean equal (TypeCode_ptr other)
	{
		return TypeCodeExceptionBase::equal (Ex::repository_id_, other);
	}

	static Boolean equivalent (TypeCode_ptr other)
	{
		return TypeCodeExceptionBase::equivalent (Ex::repository_id_, other);
	}

	static TCKind _kind (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return tk_except;
	}

	typedef Skeleton <TypeCodeException <Ex>, TypeCode> Sk;

	static const char* _id (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return Ex::repository_id_;
	}

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

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		// For exceptions, src is pointer to Data, not the exception itself.
		new (dst) Ex (reinterpret_cast <const typename Ex::Data*> (src));
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		_copy (dst, src);
	}

	using TypeCodeExceptionBase::__local_marshal;
	using TypeCodeExceptionBase::__local_unmarshal_in;
	using TypeCodeExceptionBase::__local_unmarshal_inout;
};

}
}

#endif
