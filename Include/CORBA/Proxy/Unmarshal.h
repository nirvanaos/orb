/*
~~~{.idl}
module CORBA {
module Nirvana {

/// Unmarshals data between objects in the same platform domain.
pseudo interface Unmarshal
{
	/// Returns marshal context.
	/// Used for proxy code optimization.
	attribute readonly MarshalContext context;

	void adopt_memory (::Nirvana::ConstPointer p, ::Nirvana::Size size);
	Interface unmarshal_interface (::Nirvana::ConstPointer marshal_data, RepositoryId iid);
	TypeCode unmarshal_type_code (::Nirvana::ConstPointer marshal_data);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_UNMARSHAL_H_
#define NIRVANA_ORB_UNMARSHAL_H_

#include "../Object.h"
#include "../TypeCode.h"
#include "MarshalContext.h"

namespace CORBA {

class Object;

namespace Nirvana {

class Unmarshal;
typedef I_ptr <Unmarshal> Unmarshal_ptr;
typedef I_var <Unmarshal> Unmarshal_var;
typedef I_out <Unmarshal> Unmarshal_out;

/// \interface Unmarshal
template <class T>
class Client <T, Unmarshal> :
	public T
{
public:
	MarshalContext context ();

	void adopt_memory (::Nirvana::ConstPointer p, ::Nirvana::Size size);
	NIRVANA_NODISCARD Interface* unmarshal_interface (::Nirvana::ConstPointer marshal_data, String_in iid);
	TypeCode_var unmarshal_type_code (::Nirvana::ConstPointer marshal_data);
};

BRIDGE_BEGIN (Unmarshal, CORBA_NIRVANA_REPOSITORY_ID ("Unmarshal"))
ABI_enum (*_get_context) (Bridge <Unmarshal>*, Interface*);
void (*adopt_memory) (Bridge <Unmarshal>*, ::Nirvana::ConstPointer, ::Nirvana::Size, Interface*);
Interface* (*unmarshal_interface) (Bridge <Unmarshal>*, ::Nirvana::ConstPointer, Type <String>::ABI_in, Interface*);
Interface* (*unmarshal_type_code) (Bridge <Unmarshal>*, ::Nirvana::ConstPointer, Interface*);
BRIDGE_END ()

template <class T>
MarshalContext Client <T, Unmarshal>::context ()
{
	Environment _env;
	Bridge <Marshal>& _b (T::_get_bridge (_env));
	Type <MarshalContext>::C_ret _ret = (_b._epv ().epv._get_context) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, Unmarshal>::adopt_memory (::Nirvana::ConstPointer p, ::Nirvana::Size size)
{
	Environment _env;
	Bridge <Unmarshal>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.adopt_memory) (&_b, p, size, &_env);
	_env.check ();
}

template <class T>
NIRVANA_NODISCARD Interface* Client <T, Unmarshal>::unmarshal_interface (::Nirvana::ConstPointer marshal_data, String_in iid)
{
	Environment _env;
	Bridge <Unmarshal>& _b (T::_get_bridge (_env));
	Interface* _ret = (_b._epv ().epv.unmarshal_interface) (&_b, marshal_data, &iid, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeCode_var Client <T, Unmarshal>::unmarshal_type_code (::Nirvana::ConstPointer marshal_data)
{
	Environment _env;
	Bridge <Unmarshal>& _b (T::_get_bridge (_env));
	I_ret <TypeCode> _ret = (_b._epv ().epv.unmarshal_type_code) (&_b, marshal_data, &_env);
	_env.check ();
	return _ret;
}

class Unmarshal : public ClientInterface <Unmarshal>
{};

}
}

#endif
