// ObjectFactory Interface to the core object functionality.
#ifndef NIRVANA_ORB_OBJECTFACTORY_H_
#define NIRVANA_ORB_OBJECTFACTORY_H_

#include "ServantBase.h"
#include "ReferenceCounter.h"
#include "DynamicServant.h"

namespace CORBA {
namespace Nirvana {

class ObjectFactory;
typedef I_ptr <ObjectFactory> ObjectFactory_ptr;
typedef I_var <ObjectFactory> ObjectFactory_var;
typedef I_out <ObjectFactory> ObjectFactory_out;

BRIDGE_BEGIN (ObjectFactory)
Interface* (*create_servant) (Bridge <ObjectFactory>*, Interface*, EnvironmentBridge*);
Interface* (*create_local_object) (Bridge <ObjectFactory>*, Interface*, EnvironmentBridge*);
Interface* (*create_reference_counter) (Bridge <ObjectFactory>*, Interface*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ObjectFactory> :
	public T
{
public:
	ServantBase_var create_servant (I_in <ServantBase> servant);
	Object_var create_local_object (I_in <AbstractBase> base);
	ReferenceCounter_var create_reference_counter (I_in <DynamicServant> dynamic);
};

template <class T>
ServantBase_var Client <T, ObjectFactory>::create_servant (I_in <ServantBase> servant)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <ServantBase> _ret = (_b._epv ().epv.create_servant) (&_b, &servant, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Object_var Client <T, ObjectFactory>::create_local_object (I_in <AbstractBase> base)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <Object> _ret = (_b._epv ().epv.create_local_object) (&_b, &base, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ReferenceCounter_var Client <T, ObjectFactory>::create_reference_counter (I_in <DynamicServant> dynamic)
{
	Environment _env;
	Bridge <ObjectFactory>& _b (T::_get_bridge (_env));
	I_ret <ReferenceCounter> _ret = (_b._epv ().epv.create_reference_counter) (&_b, &dynamic, &_env);
	_env.check ();
	return _ret;
}

class ObjectFactory : public ClientInterface <ObjectFactory>
{};

}
}

#endif
