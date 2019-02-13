// ObjectAdapter Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTADAPTER_C_H_
#define NIRVANA_ORB_OBJECTADAPTER_C_H_

#include "ServantBase_c.h"

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

	static const Char interface_id_ [];

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

// Global System interface must derive from ObjectAdapter
extern ObjectAdapter_ptr g_system;

}
}

#endif
