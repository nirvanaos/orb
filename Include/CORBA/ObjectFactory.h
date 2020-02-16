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

BRIDGE_BEGIN (ObjectFactory)
Interface* (*create_servant) (Bridge <ObjectFactory>*, Interface*, Interface*, EnvironmentBridge*);
Interface* (*create_local_object) (Bridge <ObjectFactory>*, Interface*, Interface*, EnvironmentBridge*);
Interface* (*create_reference_counter) (Bridge <ObjectFactory>*, Interface*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ObjectFactory> :
	public T
{
public:
	PortableServer::Servant create_servant (Interface* servant, Interface* dynamic);
	LocalObject_ptr create_local_object (Interface* base, Interface* dynamic);
	ReferenceCounter_ptr create_reference_counter (Interface* dynamic);
};

template <class T>
PortableServer::Servant Client <T, ObjectFactory>::create_servant (Interface* servant, Interface* dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <PortableServer::ServantBase> _ret = (_b._epv ().epv.create_servant) (&_b, servant, dynamic, &_env);
	_env.check ();
	return _ret;
}

template <class T>
LocalObject_ptr Client <T, ObjectFactory>::create_local_object (Interface* base, Interface* dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <LocalObject> _ret = (_b._epv ().epv.create_local_object) (&_b, base, dynamic, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ReferenceCounter_ptr Client <T, ObjectFactory>::create_reference_counter (Interface* dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <ReferenceCounter> _ret = (_b._epv ().epv.create_reference_counter) (&_b, dynamic, &_env);
	_env.check ();
	return _ret;
}

class ObjectFactory : public ClientInterface <ObjectFactory>
{};

}
}

#endif
