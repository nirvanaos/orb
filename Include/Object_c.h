// Nirvana project
// Object Request Broker
// CORBA::Object interface
#ifndef NIRVANA_ORB_OBJECT_C_H_
#define NIRVANA_ORB_OBJECT_C_H_

#include "AbstractBase_c.h"

namespace CORBA {

class ImplementationDef;
//typedef Nirvana::T_ptr <ImplementationDef> ImplementationDef_ptr;
typedef Nirvana::Bridge <ImplementationDef>* ImplementationDef_ptr; // Not defined yet

class InterfaceDef;
//typedef Nirvana::T_ptr <InterfaceDef> InterfaceDef_ptr;
typedef Nirvana::Bridge <InterfaceDef>* InterfaceDef_ptr; // Not defined yet

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
			Bridge <AbstractBase>* (*CORBA_AbstractBase) (Bridge <Object>*, EnvironmentBridge*);
		}
		base;

		struct
		{
			Bridge <ImplementationDef>* (*get_implementation) (Bridge <Object>*, EnvironmentBridge*);
			Bridge <InterfaceDef>* (*get_interface) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_a) (Bridge <Object>*, const Char* type_id, EnvironmentBridge*);
			Boolean (*non_existent) (Bridge <Object>*, EnvironmentBridge*);
			Boolean (*is_equivalent) (Bridge <Object>*, Bridge <Object>*, EnvironmentBridge*);
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
	Bridge <Object>& _b = ClientBase <T, Object>::_bridge ();
	ImplementationDef_ptr _ret ((_b._epv ().epv.get_implementation) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, Object>::_get_interface ()
{
	Environment _env;
	Bridge <Object>& _b = ClientBase <T, Object>::_bridge ();
	InterfaceDef_ptr _ret ((_b._epv ().epv.get_interface) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <Object>& _b = ClientBase <T, Object>::_bridge ();
	Boolean _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_non_existent ()
{
	Environment _env;
	Bridge <Object>& _b = ClientBase <T, Object>::_bridge ();
	Boolean _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_equivalent (Object_ptr other_object)
{
	Environment _env;
	Bridge <Object>& _b = ClientBase <T, Object>::_bridge ();
	Boolean _ret = (_b._epv ().epv.is_equivalent) (&_b, other_object, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ULong Client <T, Object>::_hash (ULong maximum)
{
	Environment _env;
	Bridge <Object>& _b = ClientBase <T, Object>::_bridge ();
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

	operator Bridge <AbstractBase>& ()
	{
		Environment _env;
		Bridge <AbstractBase>* _ret = (_epv ().base.CORBA_AbstractBase) (this, &_env);
		_env.check ();
		assert (_ret);
		return *_ret;
	}
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
