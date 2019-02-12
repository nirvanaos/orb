// ObjectAdapter Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTADAPTER_H_
#define NIRVANA_ORB_OBJECTADAPTER_H_

#include "ServantBase.h"

namespace CORBA {
namespace Nirvana {

class ObjectAdapter;
typedef T_ptr <ObjectAdapter> ObjectAdapter_ptr;

struct ServantLinks
{
	Bridge <ServantBase>* servant_base;
	Bridge <Object>* object;
};

template <>
class Bridge <ObjectAdapter> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <AbstractBase>* (*CORBA_AbstractBase) (Bridge <ObjectAdapter>*, EnvironmentBridge*);
		}
		base;

		struct
		{
			ServantLinks* (*create_servant) (Bridge <ObjectAdapter>*, Bridge <ServantBase>*, const Char*, EnvironmentBridge*);
			void (*destroy_servant) (Bridge <ObjectAdapter>*, ServantLinks*, EnvironmentBridge*);
			void (*activate_object) (Bridge <ObjectAdapter>*, ServantLinks*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char* _primary_interface ()
	{
		return NIRVANA_REPOSITORY_ID (ObjectAdapter);
	}

protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
	{}
};

template <class T>
class Client <T, ObjectAdapter> :
	public ClientBase <T, ObjectAdapter>
{
public:
	ServantLinks* create_servant (ServantBase_ptr servant, const Char* type_id);
	void destroy_servant (ServantLinks*);
	void activate_object (ServantLinks* servant);
};

class ObjectAdapter :
	public ClientInterface <ObjectAdapter>,
	public Client <ObjectAdapter, AbstractBase>
{
public:
	typedef ObjectAdapter_ptr _ptr_type;

	operator AbstractBase& ()
	{
		Environment _env;
		AbstractBase* _ret = static_cast <AbstractBase*> ((_epv ().base.CORBA_AbstractBase) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

template <class T>
ServantLinks* Client <T, ObjectAdapter>::create_servant (ServantBase_ptr servant, const Char* type_id)
{
	Environment _env;
	Bridge <ObjectAdapter>& _b = ClientBase <T, ObjectAdapter>::_bridge ();
	ServantLinks* _ret = (_b._epv ().epv.create_servant) (&_b, servant, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ObjectAdapter>::destroy_servant (ServantLinks* servant)
{
	Environment _env;
	Bridge <ObjectAdapter>& _b = ClientBase <T, ObjectAdapter>::_bridge ();
	(_b._epv ().epv.destroy_servant) (&_b, servant, &_env);
	_env.check ();
}

template <class T>
void Client <T, ObjectAdapter>::activate_object (ServantLinks* servant)
{
	Environment _env;
	Bridge <ObjectAdapter>& _b = ClientBase <T, ObjectAdapter>::_bridge ();
	(_b._epv ().epv.activate_object) (&_b, servant, &_env);
	_env.check ();
}

// ObjectAdapter skeleton
template <class S>
class Skeleton <S, ObjectAdapter>
{
public:
	static const typename Bridge <ObjectAdapter>::EPV epv_;
	
	template <class Base>
	static Bridge <Interface>* _find_interface (Base& base, const Char* id)
	{
		if (RepositoryId::compatible (Bridge <ObjectAdapter>::_primary_interface (), id))
			return &static_cast <Bridge <ObjectAdapter>&> (base);
		else
			return nullptr;
	}

protected:
	static ServantLinks* _create_servant (Bridge <ObjectAdapter>* obj, Bridge <ServantBase>* servant, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj).create_servant (servant, type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static void _destroy_servant (Bridge <ObjectAdapter>* obj, ServantLinks* servant, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj).destroy_servant (servant);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}

	static void _activate_object (Bridge <ObjectAdapter>* obj, ServantLinks* servant, EnvironmentBridge* env)
	{
		try {
			S::_implementation (obj).activate_object (servant);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
	}
};

template <class S>
const Bridge <ObjectAdapter>::EPV Skeleton <S, ObjectAdapter>::epv_ = {
	{ // interface
		S::template _duplicate <ObjectAdapter>,
		S::template _release <ObjectAdapter>
	},
	{ // base
		S::template _wide <AbstractBase, ObjectAdapter>
	},
	{ // epv
		S::_create_servant,
		S::_destroy_servant,
		S::_activate_object
	}
};

// Global System interface must derive from ObjectAdapter
extern ObjectAdapter_ptr g_system;

}
}

#endif
