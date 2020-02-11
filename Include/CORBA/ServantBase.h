// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_H_
#define NIRVANA_ORB_SERVANTBASE_H_

#include "Object.h"
#include "ReferenceCounter.h"

namespace PortableServer {

class POA;
typedef ::CORBA::Nirvana::T_ptr <POA> POA_ptr;
typedef ::CORBA::Nirvana::T_var <POA> POA_var;
typedef ::CORBA::Nirvana::T_out <POA> POA_out;

class ServantBase;
typedef ::CORBA::Nirvana::T_ptr <ServantBase> Servant;
typedef ::CORBA::Nirvana::T_var <ServantBase> ServantBase_var;
typedef ::CORBA::Nirvana::T_out <ServantBase> ServantBase_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge <PortableServer::ServantBase> :
	public BridgeMarshal <PortableServer::ServantBase>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
			BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
			BASE_STRUCT_ENTRY (ReferenceCounter, _ReferenceCounter)
		}
		base;

		struct
		{
			BridgeMarshal <PortableServer::POA>* (*default_POA) (Bridge <PortableServer::ServantBase>*, EnvironmentBridge*);
			BridgeMarshal <InterfaceDef>* (*get_interface) (Bridge <PortableServer::ServantBase>*, EnvironmentBridge*);
			Boolean (*is_a) (Bridge <PortableServer::ServantBase>*, const Char* type_id, EnvironmentBridge*);
			Boolean (*non_existent) (Bridge <PortableServer::ServantBase>*, EnvironmentBridge*);
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
		BridgeMarshal <PortableServer::ServantBase> (epv.interface)
	{}
};

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
	T_ret <::PortableServer::POA> _ret = (_b._epv ().epv.default_POA) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, PortableServer::ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge <PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	T_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
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
{};

}

#endif
