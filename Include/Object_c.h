// Nirvana project
// Object Request Broker
// CORBA::Object interface
#ifndef NIRVANA_ORB_OBJECT_C_H_
#define NIRVANA_ORB_OBJECT_C_H_

#include "AbstractBase_c.h"

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
		}
		base;

		struct
		{
			BridgeMarshal <ImplementationDef>* (*get_implementation) (Bridge <Object>*, EnvironmentBridge*);
			BridgeMarshal <InterfaceDef>* (*get_interface) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_a) (Bridge <Object>*, const Char* type_id, EnvironmentBridge*);
			Boolean (*non_existent) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_equivalent) (Bridge <Object>*, BridgeMarshal <Object>*, EnvironmentBridge*);
			ULong (*hash) (Bridge <Object>*, ULong maximum, EnvironmentBridge*);
			// TODO: Other Object operations shall be here...
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	friend class CORBA::AbstractBase; // TODO: Does it really need?

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
	Bridge <Object>& _b (*this);
	ImplementationDef_var _ret ((_b._epv ().epv.get_implementation) (&_b, &_env));
	_env.check ();
	return _ret._retn ();
}

template <class T>
InterfaceDef_ptr Client <T, Object>::_get_interface ()
{
	Environment _env;
	Bridge <Object>& _b (*this);
	InterfaceDef_var _ret ((_b._epv ().epv.get_interface) (&_b, &_env));
	_env.check ();
	return _ret._retn ();
}

template <class T>
Boolean Client <T, Object>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <Object>& _b (*this);
	Boolean _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_non_existent ()
{
	Environment _env;
	Bridge <Object>& _b (*this);
	Boolean _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_equivalent (Object_ptr other_object)
{
	Environment _env;
	Bridge <Object>& _b (*this);
	Boolean _ret = (_b._epv ().epv.is_equivalent) (&_b, other_object, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, Object>::_hash (ULong maximum)
{
	Environment _env;
	Bridge <Object>& _b (*this);
	ULong _ret = (_b._epv ().epv.hash) (&_b, maximum, &_env);
	_env.check ();
	return _ret;
}

// TODO: Other Object operations shall be here...

}

class Object :
	public Nirvana::ClientInterfacePrimary <Object>,
	public Nirvana::ClientInterfaceBase <Object, AbstractBase>
{};

inline Object_ptr AbstractBase::_to_object ()
{
	return _query_interface <Object> ();
}

namespace Nirvana {
class LocalObject : public Object
{};

}

namespace Nirvana {

//! \class	ClientInterface
//!
//! \brief	A client interface, derived from Object.
//!
//! \tparam	I	Interface.

template <class I>
class ClientInterface :
	public ClientInterfacePseudo <I>
{
public:
	static T_ptr <I> _narrow (Object_ptr obj)
	{
		return obj->_query_interface <I> ();
	}
};

//! \class	ClientInterfaceAbstract
//!
//! \brief	Abstract client interface.
//!
//! \tparam	I	Interface.

template <class I>
class ClientInterfaceAbstract :
	public ClientInterface <I>
{
public:
	static T_ptr <I> _narrow (Object_ptr obj)
	{
		return ClientInterface <I>::_narrow (obj);
	}

	static T_ptr <I> _narrow (ValueBase*);
};

}
}

#endif
