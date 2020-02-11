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

namespace Nirvana {

class SynchronizationDomain;

}

namespace CORBA {

typedef Nirvana::Interface ImplementationDef; // Not defined yet
typedef Nirvana::T_ptr <ImplementationDef> ImplementationDef_ptr;
typedef Nirvana::T_var <ImplementationDef> ImplementationDef_var;
typedef Nirvana::T_out <ImplementationDef> ImplementationDef_out;
typedef Nirvana::T_inout <ImplementationDef> ImplementationDef_inout;

typedef Nirvana::Interface InterfaceDef; // Not defined yet
typedef Nirvana::T_ptr <InterfaceDef> InterfaceDef_ptr;
typedef Nirvana::T_var <InterfaceDef> InterfaceDef_var;
typedef Nirvana::T_out <InterfaceDef> InterfaceDef_out;
typedef Nirvana::T_inout <InterfaceDef> InterfaceDef_inout;

namespace Nirvana {

template <>
class Bridge <Object> :
	public BridgeMarshal <Object>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY(CORBA::AbstractBase, CORBA_AbstractBase)
		} base;

		struct
		{
			BridgeMarshal <ImplementationDef>* (*get_implementation) (Bridge <Object>*, EnvironmentBridge*);
			BridgeMarshal <InterfaceDef>* (*get_interface) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_a) (Bridge <Object>*, const Char* type_id, EnvironmentBridge*);
			Boolean (*non_existent) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_equivalent) (Bridge <Object>*, BridgeMarshal <Object>*, EnvironmentBridge*);
			ULong (*hash) (Bridge <Object>*, ULong maximum, EnvironmentBridge*);
			// TODO: Other Object operations shall be here...
		} epv;

		struct
		{
			BridgeMarshal <::Nirvana::SynchronizationDomain>* (*sync_domain) (Bridge <Object>*, EnvironmentBridge*);
			const IOP::IOR* (*object_reference) (Boolean local);
			const void* (*call_target) ();
		} internal;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <Object> (epv.interface)
	{}
};

template <class T>
class Client <T, Object> :
	public T
{
public:
	ImplementationDef_ptr _get_implementation ();
	InterfaceDef_ptr _get_interface ();
	Boolean _is_a (const Char* type_id);
	Boolean _non_existent ();
	Boolean _is_equivalent (Object_ptr other_object);
	ULong _hash (ULong maximum);
	// TODO: Other Object operations shall be here...
};

template <class T>
ImplementationDef_ptr Client <T, Object>::_get_implementation ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	T_ret <ImplementationDef> _ret = (_b._epv ().epv.get_implementation) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, Object>::_get_interface ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	T_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_non_existent ()
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_equivalent (Object_ptr other_object)
{
	Environment _env;
	Bridge <Object>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.is_equivalent) (&_b, other_object, &_env);
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

/*
class Object : public Nirvana::ClientInterface <Object, AbstractBase>
{};*/
class Object : 
	public Nirvana::ClientInterfacePrimary <Object>,
	public Nirvana::ClientBase <Object, AbstractBase> // AbstractBase operations are not available directly on Object_ptr.
{};

inline Object_ptr AbstractBase::_to_object ()
{
	return Object::_duplicate (_query_interface <Object> ());
}

namespace Nirvana {

template <class Primary>
class ClientInterfaceBase <Primary, Object> :
	public Client <ClientBase <Primary, Object>, Object>
{
public:
	static T_ptr <Primary> _narrow (Object_ptr obj)
	{
		return Primary::_duplicate (AbstractBase_ptr (obj)->_query_interface <Primary> ());
	}
};

}

}

#endif
