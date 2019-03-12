// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_C_H_
#define NIRVANA_ORB_SERVANTBASE_C_H_

#include "Object_c.h"

namespace PortableServer {

class POA;
typedef ::CORBA::Nirvana::T_ptr <POA> POA_ptr;
typedef ::CORBA::Nirvana::T_var <POA> POA_var;
typedef ::CORBA::Nirvana::T_out <POA> POA_out;

}

namespace CORBA {
namespace Nirvana {

class ServantBase;
typedef T_ptr <ServantBase> ServantBase_ptr;
typedef T_var <ServantBase> ServantBase_var;
typedef T_out <ServantBase> ServantBase_out;

template <>
class Bridge <ServantBase> :
	public BridgeMarshal <ServantBase>
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
			BridgeMarshal <::PortableServer::POA>* (*default_POA) (Bridge <ServantBase>*, EnvironmentBridge*);
			BridgeMarshal <InterfaceDef>* (*get_interface) (Bridge <ServantBase>*, EnvironmentBridge*);
			Boolean (*is_a) (Bridge <ServantBase>*, const Char* type_id, EnvironmentBridge*);
			Boolean (*non_existent) (Bridge <ServantBase>*, EnvironmentBridge*);
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
		BridgeMarshal <ServantBase> (epv.interface)
	{}
};

template <class T>
class Client <T, ServantBase> :
	public T
{
public:
	::PortableServer::POA_ptr _default_POA ();
	InterfaceDef_ptr _get_interface ();
	Boolean _is_a (const Char* type_id);
	Boolean _non_existent ();
};

template <class T>
::PortableServer::POA_ptr Client <T, ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <ServantBase>& _b (*this);
	::PortableServer::POA_var _ret ((_b._epv ().epv.default_POA) (&_b, &_env));
	_env.check ();
	return _ret._retn ();
}

template <class T>
InterfaceDef_ptr Client <T, ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge <ServantBase>& _b (*this);
	InterfaceDef_var _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret._retn ();
}

template <class T>
Boolean Client <T, ServantBase>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <ServantBase>& _b (*this);
	Boolean _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge <ServantBase>& _b (*this);
	Boolean _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

class ServantBase :
	public ::CORBA::Nirvana::ClientInterfacePseudo <ServantBase>,
	public ::CORBA::Nirvana::ClientInterfaceBase <ServantBase, ::CORBA::AbstractBase>
{};

}
}

#endif
