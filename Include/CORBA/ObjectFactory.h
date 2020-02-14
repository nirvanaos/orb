// ObjectFactory Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTFACTORY_H_
#define NIRVANA_ORB_OBJECTFACTORY_H_

#include "ServantBase.h"
#include "DynamicServant.h"
#include "LocalObject.h"
#include <Nirvana/OLF.h>

namespace CORBA {
namespace Nirvana {

class ObjectFactory;
typedef I_ptr <ObjectFactory> ObjectFactory_ptr;
typedef I_var <ObjectFactory> ObjectFactory_var;
typedef I_out <ObjectFactory> ObjectFactory_out;

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
			BridgeMarshal <PortableServer::ServantBase>* (*create_servant) (Bridge <ObjectFactory>*, BridgeMarshal <PortableServer::ServantBase>*, BridgeMarshal <DynamicServant>*, EnvironmentBridge*);
			BridgeMarshal <LocalObject>* (*create_local_object) (Bridge <ObjectFactory>*, BridgeMarshal <AbstractBase>*, BridgeMarshal <DynamicServant>*, EnvironmentBridge*);
			BridgeMarshal <ReferenceCounter>* (*create_reference_counter) (Bridge <ObjectFactory>*, BridgeMarshal <DynamicServant>*, EnvironmentBridge*);
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
	PortableServer::Servant create_servant (BridgeMarshal <PortableServer::ServantBase>* servant, BridgeMarshal <DynamicServant>* dynamic);
	LocalObject_ptr create_local_object (BridgeMarshal <AbstractBase>* base, BridgeMarshal <DynamicServant>* dynamic);
	ReferenceCounter_ptr create_reference_counter (BridgeMarshal <DynamicServant>* dynamic);
};

template <class T>
PortableServer::Servant Client <T, ObjectFactory>::create_servant (BridgeMarshal <PortableServer::ServantBase>* servant, BridgeMarshal <DynamicServant>* dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <PortableServer::ServantBase> _ret = (_b._epv ().epv.create_servant) (&_b, servant, dynamic, &_env);
	_env.check ();
	return _ret;
}

template <class T>
LocalObject_ptr Client <T, ObjectFactory>::create_local_object (BridgeMarshal <AbstractBase>* base, BridgeMarshal <DynamicServant>* dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <LocalObject> _ret = (_b._epv ().epv.create_local_object) (&_b, base, dynamic, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ReferenceCounter_ptr Client <T, ObjectFactory>::create_reference_counter (BridgeMarshal <DynamicServant>* dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <ReferenceCounter> _ret = (_b._epv ().epv.create_reference_counter) (&_b, dynamic, &_env);
	_env.check ();
	return _ret;
}

class ObjectFactory : public ClientInterface <ObjectFactory>
{};

extern const ::Nirvana::ImportInterfaceT <ObjectFactory> g_object_factory;

}
}

#endif
