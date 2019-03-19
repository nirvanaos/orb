// ObjectFactory Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTFACTORY_C_H_
#define NIRVANA_ORB_OBJECTFACTORY_C_H_

#include "ServantBase_c.h"
#include "DynamicServant_c.h"

namespace CORBA {
namespace Nirvana {

class ObjectFactory;
typedef T_ptr <ObjectFactory> ObjectFactory_ptr;
typedef T_var <ObjectFactory> ObjectFactory_var;
typedef T_out <ObjectFactory> ObjectFactory_out;

template <>
class Bridge <ObjectFactory> :
	public BridgeMarshal <ObjectFactory>
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
			BridgeMarshal <PortableServer::ServantBase>* (*create_servant) (Bridge <ObjectFactory>*, BridgeMarshal <PortableServer::ServantBase>*, BridgeMarshal <DynamicServant>*, EnvironmentBridge*);
			BridgeMarshal <Object>* (*create_local_object) (Bridge <ObjectFactory>*, BridgeMarshal <DynamicServant>*, EnvironmentBridge*);
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
		BridgeMarshal <ObjectFactory> (epv.interface)
	{}
};

template <class T>
class Client <T, ObjectFactory> :
	public T
{
public:
	PortableServer::Servant create_servant (PortableServer::Servant servant, DynamicServant_ptr dynamic);
	Object_ptr create_local_object (DynamicServant_ptr dynamic);
};

class ObjectFactory : public ClientInterface <ObjectFactory, AbstractBase>
{};

template <class T>
PortableServer::Servant Client <T, ObjectFactory>::create_servant (PortableServer::Servant servant, DynamicServant_ptr dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	PortableServer::ServantBase_var _ret = (_b._epv ().epv.create_servant) (&_b, servant, dynamic, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
Object_ptr Client <T, ObjectFactory>::create_local_object (DynamicServant_ptr dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	Object_var _ret = (_b._epv ().epv.create_local_object) (&_b, dynamic, &_env);
	_env.check ();
	return _ret._retn ();
}

extern ObjectFactory_ptr g_object_factory;

}
}

#endif
