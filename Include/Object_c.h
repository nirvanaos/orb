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
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <AbstractBase>* (*CORBA_AbstractBase) (Bridge <Object>*, const Char* id, EnvironmentBridge*);
		}
		base;

		struct
		{
			ClientBridge <ImplementationDef>* (*get_implementation) (Bridge <Object>*, EnvironmentBridge*);
			ClientBridge <InterfaceDef>* (*get_interface) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_a) (Bridge <Object>*, const Char* type_id, EnvironmentBridge*);
			Boolean (*non_existent) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_equivalent) (Bridge <Object>*, ClientBridge <Object>*, EnvironmentBridge*);
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
		Bridge <Interface> (epv.interface)
	{}
};

template <class T>
class ClientBase <T, Object>
{
public:
	operator Object& ()
	{
		Environment _env;
		T& t = static_cast <T&> (*this);
		Bridge <Object>* _ret = (t._epv ().base.CORBA_Object) (&t, Bridge <Object>::interface_id_, &_env);
		_env.check ();
		if (!_ret)
			throw MARSHAL ();
		return static_cast <Object&> (*_ret);
	}

	operator Bridge <Object>& ()
	{
		return operator Object& ();
	}
};

template <>
class ClientBase <Object, Object> :
	public ClientBridge <Object>
{};

template <class T>
class Client <T, Object> :
	public ClientBase <T, Object>
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
	public Nirvana::ClientInterfaceBase <Object>,
	public Nirvana::Client <Object, AbstractBase>
{
public:
	typedef Object_ptr _ptr_type;
};

inline Object_ptr AbstractBase::_to_object ()
{
	return _find_interface <Object> ();
}

namespace Nirvana {


template <class I>
class ClientInterface :
	public ClientInterfacePseudo <I>
{
public:
	static T_ptr <I> _narrow (Object_ptr obj)
	{
		return obj->_find_interface <I> ();
	}
};

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
