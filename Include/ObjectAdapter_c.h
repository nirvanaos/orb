// ObjectAdapter Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTADAPTER_C_H_
#define NIRVANA_ORB_OBJECTADAPTER_C_H_

#include "ServantBase_c.h"
#include "DynamicServant_c.h"

namespace CORBA {
namespace Nirvana {

class ObjectAdapter;
typedef T_ptr <ObjectAdapter> ObjectAdapter_ptr;
typedef T_var <ObjectAdapter> ObjectAdapter_var;
typedef T_out <ObjectAdapter> ObjectAdapter_out;

template <>
class Bridge <ObjectAdapter> :
	public BridgeMarshal <ObjectAdapter>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
		}
		base;

		struct
		{
			BridgeMarshal <PortableServer::ServantBase>* (*create_servant) (Bridge <ObjectAdapter>*, BridgeMarshal <PortableServer::ServantBase>*, BridgeMarshal <DynamicServant>*, EnvironmentBridge*);
			BridgeMarshal <Object>* (*create_local_object) (Bridge <ObjectAdapter>*, BridgeMarshal <DynamicServant>*, EnvironmentBridge*);
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
		BridgeMarshal <ObjectAdapter> (epv.interface)
	{}
};

template <class T>
class Client <T, ObjectAdapter> :
	public T
{
public:
	PortableServer::Servant create_servant (PortableServer::Servant servant, DynamicServant_ptr dynamic);
	Object_ptr create_local_object (DynamicServant_ptr dynamic);
};

class ObjectAdapter : public ClientInterface <ObjectAdapter, AbstractBase>
{};

template <class T>
PortableServer::Servant Client <T, ObjectAdapter>::create_servant (PortableServer::Servant servant, DynamicServant_ptr dynamic)
{
	Environment _env;
	Bridge <ObjectAdapter>& _b (T::_get_bridge (_env));
	PortableServer::ServantBase_var _ret = (_b._epv ().epv.create_servant) (&_b, servant, dynamic, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
Object_ptr Client <T, ObjectAdapter>::create_local_object (DynamicServant_ptr dynamic)
{
	Environment _env;
	Bridge <ObjectAdapter>& _b (T::_get_bridge (_env));
	Object_var _ret = (_b._epv ().epv.create_local_object) (&_b, dynamic, &_env);
	_env.check ();
	return _ret._retn ();
}

extern ObjectAdapter_ptr g_object_adapter;

}
}

#endif
