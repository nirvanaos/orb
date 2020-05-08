// Nirvana project
// Object Request Broker
// CORBA::Object interface
#ifndef NIRVANA_ORB_OBJECT_H_
#define NIRVANA_ORB_OBJECT_H_

#include "AbstractBase.h"

//! \namespace	IOP
//!
//! \brief	Inter-ORB declarations.

namespace IOP {

struct IOR;

}

namespace PortableServer {

class ServantBase;
typedef ::CORBA::Nirvana::I_var <ServantBase> ServantBase_var;
typedef ::CORBA::Nirvana::I_out <ServantBase> ServantBase_out;

}

namespace CORBA {

typedef Nirvana::Interface ImplementationDef; // Not defined, unused
typedef Nirvana::I_ptr <ImplementationDef> ImplementationDef_ptr;
typedef Nirvana::I_var <ImplementationDef> ImplementationDef_var;
typedef Nirvana::I_out <ImplementationDef> ImplementationDef_out;
typedef Nirvana::I_inout <ImplementationDef> ImplementationDef_inout;

typedef Object InterfaceDef; // TODO: Not defined yet
typedef Nirvana::I_ptr <InterfaceDef> InterfaceDef_ptr;
typedef Nirvana::I_var <InterfaceDef> InterfaceDef_var;
typedef Nirvana::I_out <InterfaceDef> InterfaceDef_out;
typedef Nirvana::I_inout <InterfaceDef> InterfaceDef_inout;

namespace Nirvana {

template <>
const Char Bridge <Object>::repository_id_ [] = CORBA_REPOSITORY_ID ("Object");

template <>
struct Bridge <Object>::EPV
{
	typedef Bridge <Object> MyBridge;

	Interface::EPV header;

	struct
	{
		BASE_STRUCT_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
	} base;

	struct
	{
		Interface* (*get_interface) (Bridge <Object>*, EnvironmentBridge*);
		ABI_boolean (*is_a) (Bridge <Object>*, ABI_in <String> type_id, EnvironmentBridge*);
		ABI_boolean (*non_existent) (Bridge <Object>*, EnvironmentBridge*);
		ABI_boolean (*is_equivalent) (Bridge <Object>*, Interface*, EnvironmentBridge*);
		ULong (*hash) (Bridge <Object>*, ULong maximum, EnvironmentBridge*);
		// TODO: Other Object operations shall be here...
	} epv;

	struct
	{
		Interface* (*get_servant) (Bridge <Object>*, EnvironmentBridge*);
		const IOP::IOR* (*object_reference) (ABI_in <Boolean> local);
	} internal;
};

template <class T>
class Client <T, Object> :
	public T
{
public:
	ImplementationDef_var _get_implementation ();
	InterfaceDef_var _get_interface ();
	Boolean _is_a (String_in type_id);
	Boolean _non_existent ();
	Boolean _is_equivalent (I_in <Object> other_object);
	ULong _hash (ULong maximum);
	// TODO: Other Object operations shall be here...
};

template <class T>
ImplementationDef_var Client <T, Object>::_get_implementation ()
{
	return ImplementationDef::_nil ();
}

template <class T>
InterfaceDef_var Client <T, Object>::_get_interface ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_a (String_in type_id)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.is_a) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_non_existent ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_equivalent (I_in <Object> other_object)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.is_equivalent) (&_b, &other_object, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, Object>::_hash (ULong maximum)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.hash) (&_b, maximum, &_env);
	_env.check ();
	return _ret;
}

// TODO: Other Object operations shall be here...

}

class Object : 
	public Nirvana::ClientInterfacePrimary <Object>,
	public Nirvana::ClientBase <Object, AbstractBase> // AbstractBase operations are not available directly on Object_ptr.
{};

inline Object_var AbstractBase::_to_object ()
{
	return Object::_duplicate (_query_interface <Object> ());
}

namespace Nirvana {

template <class Primary>
class ClientInterfaceBase <Primary, Object> :
	public Client <ClientBase <Primary, Object>, Object>
{
public:
	static I_ptr <Primary> _narrow (Object_ptr obj)
	{
		return Primary::_duplicate (AbstractBase_ptr (obj)->_query_interface <Primary> ());
	}
};

}

}

#endif
