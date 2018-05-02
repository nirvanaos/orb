#ifndef NIRVANA_ORB_OBJECT_H_
#define NIRVANA_ORB_OBJECT_H_

#include "Implementation.h"
#include "ImplementationStatic.h"
#include "ImplementationPOA.h"
#include "ImplementationTied.h"

namespace PortableServer {

class POA;
//typedef CORBA::Nirvana::T_ptr <POA> POA_ptr;
typedef CORBA::Nirvana::Bridge<POA>* POA_ptr; // Not defined yet
}

namespace CORBA {

class ImplementationDef;
//typedef Nirvana::T_ptr <ImplementationDef> ImplementationDef_ptr;
typedef Nirvana::Bridge <ImplementationDef>* ImplementationDef_ptr; // Not defined yet

class InterfaceDef;
//typedef Nirvana::T_ptr <InterfaceDef> InterfaceDef_ptr;
typedef Nirvana::Bridge <InterfaceDef>* InterfaceDef_ptr; // Not defined yet

namespace Nirvana {

using PortableServer::POA_ptr;

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
			Bridge <AbstractBase>* (*AbstractBase) (Bridge <Object>*, Environment*);
		}
		base;

		struct
		{
			Bridge <ImplementationDef>* (*get_implementation) (Bridge <Object>*, Environment*);
			Bridge <InterfaceDef>* (*get_interface) (Bridge <Object>*, Environment*);
			Boolean (*is_a) (Bridge <Object>*, const Char* type_id, Environment*);
			Boolean (*non_existent) (Bridge <Object>*, Environment*);
			Boolean (*is_equivalent) (Bridge <Object>*, Bridge <Object>*, Environment*);
			// TODO: Other Object operations shall be here...
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char* _primary_interface ()
	{
		return "IDL:omg.org/CORBA/Object:1.0";
	}

	static Boolean ___is_a (const Char* type_id)
	{
		return RepositoryId::compatible(_primary_interface (), type_id);
	}

protected:
	friend class CORBA::AbstractBase;

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

	// TODO: Other Object operations shall be here...
};

template <class T>
ImplementationDef_ptr Client <T, Object>::_get_implementation ()
{
	Environment _env;
	Bridge <Object>& _b = _bridge ();
	ImplementationDef_ptr _ret ((_b._epv ().epv.get_implementation) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, Object>::_get_interface ()
{
	Environment _env;
	Bridge <Object>& _b = _bridge ();
	InterfaceDef_ptr _ret ((_b._epv ().epv.get_interface) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <Object>& _b = _bridge ();
	Boolean _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_non_existent ()
{
	Environment _env;
	Bridge <Object>& _b = _bridge ();
	Boolean _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, Object>::_is_equivalent (Object_ptr other_object)
{
	Environment _env;
	Bridge <Object>& _b = _bridge ();
	Boolean _ret = (_b._epv ().epv.is_equivalent) (&_b, other_object, &_env);
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
		AbstractBase* _ret = static_cast <AbstractBase*> ((_epv ().base.AbstractBase) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

inline Object_ptr AbstractBase::_to_object ()
{
	return Object::_duplicate (static_cast <Bridge <Object>*> (_find_interface (Bridge <Object>::_primary_interface ())));
}

namespace Nirvana {

template <class I>
T_ptr <I> ClientInterface <I>::_narrow (T_ptr <Object> obj)
{
	return _duplicate (static_cast <Bridge <I>*> (obj->_find_interface (Bridge <I>::_primary_interface ())));
}

// Default implementation of Object interface
class ObjectBase
{
public:
	static POA_ptr _default_POA ()
	{
		return 0;
	}

	static Bridge <ImplementationDef>* __get_implementation (Bridge <Object>* obj, Environment* env);
	static Boolean __is_equivalent (Bridge <Object>* obj, Bridge <Object>* other_object, Environment* env);

	static Bridge <InterfaceDef>* _get_interface (const Char* type_id)
	{
		return 0; // TODO: Get interface definition from repository.
	}

	static Boolean _non_existent ()
	{
		return FALSE;
	}
};

// Object skeleton
template <class S>
class Skeleton <S, Object> :
	public ObjectBase
{
public:
	static const typename Bridge <Object>::EPV sm_epv;

	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <Object>::_primary_interface (), id))
			return &S::_narrow	<Object> (base);
		else
			return nullptr;
	}

protected:
	static Bridge <InterfaceDef>* __get_interface (Bridge <Object>* obj, Environment* env)
	{
		try {
			return S::_object (obj)._get_interface ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static InterfaceDef_ptr _get_interface ()
	{
		return ObjectBase::_get_interface (S::_primary_interface ());
	}

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, Environment* env)
	{
		try {
			_check_pointer (type_id);
			return S::___is_a (type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __non_existent (Bridge <Object>* obj, Environment* env)
	{
		try {
			return S::_object (obj)._non_existent ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <Object>::EPV Skeleton <S, Object>::sm_epv = {
	{ // interface
		S::_duplicate <Object>,
		S::_release <Object>
	},
	{ // base
		S::_wide <AbstractBase, Object>
	},
	{ // epv
		S::__get_implementation,
		S::__get_interface,
		S::__is_a,
		S::__non_existent,
		S::__is_equivalent
	}
};

// Standard implementation

template <class S>
class InterfaceImpl <S, Object> :
	public Bridge <Object>,
	public Skeleton <S, Object>
{
public:
	T_ptr <Object> _this ()
	{
		return this;
	}

	static S& _object (Bridge <Object>* bridge)
	{
		_check_pointer (bridge, sm_epv.interface);
		return static_cast <S&> (*bridge);
	}

protected:
	InterfaceImpl () :
		Bridge <Object> (sm_epv)
	{}
};

template <class S>
class Servant <S, Object> :
	public Implementation <S, Object>
{};

// POA implementation

template <>
class ServantPOA <::CORBA::Object> :
	public ImplementationPOA <::CORBA::Object>
{
public:
	static ServantPOA <Object>& _object (Bridge <Object>* bridge)
	{
		return _implementation (bridge);
	}

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

	static Boolean __is_a (Bridge <Object>* obj, const Char* type_id, Environment* env)
	{
		try {
			_check_pointer (type_id);
			return _implementation (obj)._is_a (type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

// Static implementation

template <class S>
class InterfaceStatic <S, Object> :
	public Skeleton <S, Object>
{
	InterfaceStatic ();	// Never be instantiated
public:
	static T_ptr <Object> _this ()
	{
		return _bridge ();
	}

	static Bridge <Object>* _bridge ()
	{
		return (Bridge <Object>*)&sm_bridge;
	}

	static S& _object (Bridge <Object>* bridge)
	{
		return *(S*)0;
	}

private:
	static const typename Bridge <Object>::EPV* sm_bridge;
};

template <class S>
const typename Bridge <Object>::EPV* InterfaceStatic<S, Object>::sm_bridge = &sm_epv;

template <class S>
class ServantStatic <S, Object> :
	public ImplementationStatic <S, Object>
{};

// Tied implementation

template <class S>
class InterfaceTied <S, Object> :
	public InterfaceTiedBase <Object>,
	public Skeleton <S, Object>
{
public:
	static S& _object (Bridge <Object>* bridge)
	{
		_check_pointer (bridge, sm_epv.interface);
		return static_cast <S&> (*bridge);
	}

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
		InterfaceTiedBase <Object> (sm_epv),
		m_poa (0)
	{}

	POA_ptr m_poa;
};

}
}

#endif
