// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_H_
#define NIRVANA_ORB_SERVANTBASE_H_

#include "Object.h"
#include "ReferenceCounter.h"

namespace PortableServer {

class POA;
typedef ::CORBA::Nirvana::I_ptr <POA> POA_ptr;
typedef ::CORBA::Nirvana::I_var <POA> POA_var;
typedef ::CORBA::Nirvana::I_out <POA> POA_out;

class ServantBase;
typedef ::CORBA::Nirvana::I_ptr <ServantBase> Servant;
typedef ::CORBA::Nirvana::I_var <ServantBase> ServantBase_var;
typedef ::CORBA::Nirvana::I_out <ServantBase> ServantBase_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (PortableServer::ServantBase)
BASE_STRUCT_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BASE_STRUCT_ENTRY (ReferenceCounter, _ReferenceCounter)
BRIDGE_EPV
Interface* (*default_POA) (Bridge <PortableServer::ServantBase>*, EnvironmentBridge*);
Interface* (*get_interface) (Bridge <PortableServer::ServantBase>*, EnvironmentBridge*);
Boolean (*is_a) (Bridge <PortableServer::ServantBase>*, const Char* type_id, EnvironmentBridge*);
Boolean (*non_existent) (Bridge <PortableServer::ServantBase>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, PortableServer::ServantBase> :
	public T
{
public:
	::PortableServer::POA_ptr _default_POA ();
	InterfaceDef_ptr _get_interface ();
	Boolean _is_a (const Char* type_id);
	Boolean _non_existent ();
};

template <class T>
::PortableServer::POA_ptr Client <T, PortableServer::ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <::PortableServer::POA> _ret = (_b._epv ().epv.default_POA) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, PortableServer::ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, PortableServer::ServantBase>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, PortableServer::ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	Boolean _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace PortableServer {

class ServantBase :
	public ::CORBA::Nirvana::ClientInterface <ServantBase, ::CORBA::Nirvana::ReferenceCounter>,
	// Client methods from bases AbstractBase and Object are not available directly on pointer to ServantBase.
	public ::CORBA::Nirvana::ClientBase <ServantBase, ::CORBA::Object>,
	public ::CORBA::Nirvana::ClientBase <ServantBase, ::CORBA::AbstractBase>
{
public:
	static Servant _check (Interface* bridge)
	{
		return static_cast <ServantBase*> (Interface::_check (bridge, check_interface_id_));
	}

	static const ::CORBA::Char check_interface_id_ [];
};

}

#endif
