// Nirvana project
// Object Request Broker
// CORBA::Object interface
#ifndef NIRVANA_ORB_OBJECT_H_
#define NIRVANA_ORB_OBJECT_H_

#include "AbstractBase.h"

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

	Bridge ()
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
	public Nirvana::ClientInterface <Object>,
	public Nirvana::Client <Object, AbstractBase>
{
public:
	typedef Object_ptr _ptr_type;

	operator AbstractBase& ()
	{
		Environment _env;
		AbstractBase* _ret = static_cast <AbstractBase*> ((_epv ().base.CORBA_AbstractBase) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

inline Object_ptr AbstractBase::_to_object ()
{
	return static_cast <Bridge <Object>*> (_find_interface (Bridge <Object>::interface_id_));
}

namespace Nirvana {

template <class I>
T_ptr <I> ClientInterface <I>::_narrow (T_ptr <Object> obj)
{
	return static_cast <Bridge <I>*> (obj->_find_interface (Bridge <I>::interface_id_));
}

// Object skeleton
template <class S>
class Skeleton <S, Object>
{
public:
	static const typename Bridge <Object>::EPV epv_;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <Object>::interface_id_, id))
			return &static_cast <Bridge <Object>&> (base);
		else
			return nullptr;
	}

protected:
	static Bridge <ImplementationDef>* __get_implementation (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._get_implementation ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Bridge <InterfaceDef>* __get_interface (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._get_interface ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_a (type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __non_existent (Bridge <Object>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __is_equivalent (Bridge <Object>* obj, Bridge <Object>* other_object, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_equivalent ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static ULong __hash (Bridge <Object>* obj, ULong maximum, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._hash (maximum);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <Object>::EPV Skeleton <S, Object>::epv_ = {
	{ // interface
		S::template _duplicate <Object>,
		S::template _release <Object>
	},
	{ // base
		S::template _wide <AbstractBase, Object>
	},
	{ // epv
		S::__get_implementation,
		S::__get_interface,
		S::__is_a,
		S::__non_existent,
		S::__is_equivalent,
		S::__hash
	}
};

// POA implementation
/*
template <>
class ServantPOA < ::CORBA::Object> :
	public ImplementationPOA < ::CORBA::Object>
{
public:
	virtual POA_ptr _default_POA ()
	{
		return ObjectBase::_default_POA ();
	}

	virtual InterfaceDef_ptr _get_interface ()
	{
		return ObjectBase::_get_interface (_primary_interface ());
	}

	virtual Boolean _non_existent ()
	{
		return ObjectBase::_non_existent ();
	}

	virtual Boolean _is_a (const Char* type_id)
	{
		return Skeleton <ServantPOA < ::CORBA::Object>, ::CORBA::Object>::_is_a (type_id);
	}
};
/*
// Tied implementation

template <class S>
class InterfaceTied <S, Object> :
	public InterfaceTiedBase <Object>,
	public Skeleton <S, Object>
{
public:
	Boolean _non_existent () const
	{
		return !static_cast <const S&> (*this)._tied_object ();
	}

	POA_ptr _default_POA () const
	{
		return 0;	// TODO: Implement.
	}

	void _default_POA (POA_ptr);	// TODO: Implement.

protected:
	InterfaceTied <S, Object> () :
		InterfaceTiedBase <Object> (Skeleton <S, Object>::epv_),
		poa_ (0)
	{}

	POA_ptr poa_;
};
*/
}
}

#endif
