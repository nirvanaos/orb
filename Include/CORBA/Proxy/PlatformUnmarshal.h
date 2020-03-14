/*
~~~{.idl}
module CORBA {
module Nirvana {

/// Unmarshals data between objects in the same platform domain.
pseudo interface PlatformUnmarshal
{
	/// Returns marshal context.
	/// Used for proxy code optimization.
	attribute readonly PlatformMarshalContext context;

	void adopt_memory (::Nirvana::Pointer p, ::Nirvana::Size size);
	Interface unmarshal_interface (::Nirvana::ConstPointer marshal_data, RepositoryId iid);
	TypeCode unmarshal_type_code (::Nirvana::ConstPointer marshal_data);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_PLATFORMUNMARSHAL_H_
#define NIRVANA_ORB_PLATFORMUNMARSHAL_H_

#include "../Object.h"
#include "../TypeCode.h"
#include "PlatformMarshalContext.h"

namespace CORBA {

class Object;

namespace Nirvana {

class PlatformUnmarshal;
typedef I_ptr <PlatformUnmarshal> PlatformUnmarshal_ptr;
typedef I_var <PlatformUnmarshal> PlatformUnmarshal_var;
typedef I_out <PlatformUnmarshal> PlatformUnmarshal_out;

/// \interface PlatformUnmarshal
template <class T>
class Client <T, PlatformUnmarshal> :
	public T
{
public:
	PlatformMarshalContext context ();

	void adopt_memory (::Nirvana::Pointer p, ::Nirvana::Size size);
	NIRVANA_NODISCARD Interface* unmarshal_interface (::Nirvana::ConstPointer marshal_data, String_in iid);
	TypeCode_var unmarshal_type_code (::Nirvana::ConstPointer marshal_data);
};

BRIDGE_BEGIN (PlatformUnmarshal, CORBA_NIRVANA_REPOSITORY_ID ("PlatformUnmarshal"))
void (*adopt_memory) (Bridge <PlatformUnmarshal>*, ::Nirvana::Pointer, ::Nirvana::Size, EnvironmentBridge*);
Interface* (*unmarshal_interface) (Bridge <PlatformUnmarshal>*, ::Nirvana::ConstPointer, Type <String>::ABI_in, EnvironmentBridge*);
Interface* (*unmarshal_type_code) (Bridge <PlatformUnmarshal>*, ::Nirvana::ConstPointer, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
void Client <T, PlatformUnmarshal>::adopt_memory (::Nirvana::Pointer p, ::Nirvana::Size size)
{
	Environment _env;
	Bridge <PlatformUnmarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.adopt_memory) (&_b, p, size, &_env);
	_env.check ();
}

template <class T>
NIRVANA_NODISCARD Interface* Client <T, PlatformUnmarshal>::unmarshal_interface (::Nirvana::ConstPointer marshal_data, String_in iid)
{
	Environment _env;
	Bridge <PlatformUnmarshal>& _b (T::_get_bridge (_env));
	Interface* _ret = (_b._epv ().epv.unmarshal_interface) (&_b, marshal_data, &iid, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, PlatformUnmarshal>::unmarshal_type_code (::Nirvana::ConstPointer marshal_data)
{
	Environment _env;
	Bridge <PlatformUnmarshal>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.unmarshal_type_code) (&_b, marshal_data, &_env);
	_env.check ();
	return _ret;
}

class PlatformUnmarshal : public ClientInterface <PlatformUnmarshal>
{};

}
}

#endif
