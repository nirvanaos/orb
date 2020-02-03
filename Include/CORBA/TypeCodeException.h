#ifndef NIRVANA_ORB_TYPECODEEXCEPTION_H_
#define NIRVANA_ORB_TYPECODEEXCEPTION_H_

#include "TypeCodeImpl.h"

namespace CORBA {
namespace Nirvana {

class TypeCodeExceptionBase :
	public TypeCodeBase
{
public:
	static Boolean equal (const char* id, TypeCode_ptr other);
	static Boolean equivalent (const char* id, TypeCode_ptr other);

	static Identifier member_name (ULong index)
	{
		throw_Bounds ();
	}

	TypeCode_ptr member_type (ULong index)
	{
		throw_Bounds ();
	}
};

template <class Ex>
class TypeCodeException :
	public TypeCodeExceptionBase,
	public TypeCodeOps <Ex>,
	public ServantStatic <TypeCodeException <Ex>, TypeCode>
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

	static BridgeMarshal <TypeCode>* _get_compact_typecode (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return InterfaceStatic <TypeCodeException <Ex>, TypeCode>::_bridge ();
	}

	static TCKind _kind (Bridge <TypeCode>* _b, EnvironmentBridge* _env)
	{
		return tk_except;
	}

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

	static void _copy (::Nirvana::Pointer dst, ::Nirvana::ConstPointer src)
	{
		// For exceptions, src is pointer to Data, not the exception itself.
		new (dst) Ex (reinterpret_cast <const typename Ex::Data*> (src));
	}

	static void _move (::Nirvana::Pointer dst, ::Nirvana::Pointer src)
	{
		_copy (dst, src);
	}
};

}
}

#endif
