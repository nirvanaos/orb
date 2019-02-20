// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_C_H_
#define NIRVANA_ORB_SERVANTBASE_C_H_

#include "POA_c.h"

namespace CORBA {
namespace Nirvana {

class ServantBase;
typedef T_ptr <ServantBase> ServantBase_ptr;
typedef T_var <ServantBase> ServantBase_var;
typedef T_out <ServantBase> ServantBase_out;

using PortableServer::POA;
using PortableServer::POA_ptr;

template <>
class Bridge <ServantBase> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <::CORBA::AbstractBase>* (*CORBA_AbstractBase) (Bridge <ServantBase>*, EnvironmentBridge*);
		}
		base;

		struct
		{
			Bridge <POA>* (*default_POA) (Bridge <ServantBase>*, EnvironmentBridge*);
			Bridge <InterfaceDef>* (*get_interface) (Bridge <ServantBase>*, EnvironmentBridge*);
			Boolean (*is_a) (Bridge <ServantBase>*, const Char* type_id, EnvironmentBridge*);
			Boolean (*non_existent) (Bridge <ServantBase>*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}
};

template <class T>
class Client <T, ServantBase> :
	public ClientBase <T, ServantBase>
{
public:
	POA_ptr _default_POA ();
	InterfaceDef_ptr _get_interface ();
	Boolean _is_a (const Char* type_id);
	Boolean _non_existent ();
};

class ServantBase :
	public ClientInterfacePseudo <ServantBase>,
	public Client <ServantBase, ::CORBA::AbstractBase>
{
public:
	typedef ServantBase_ptr _ptr_type;

	operator ::CORBA::AbstractBase& ()
	{
		Environment _env;
		::CORBA::AbstractBase* _ret = static_cast <::CORBA::AbstractBase*> ((_epv ().base.CORBA_AbstractBase) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

template <class T>
POA_ptr Client <T, ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <ServantBase>& _b = ClientBase <T, ServantBase>::_bridge ();
	POA_ptr _ret ((_b._epv ().epv.default_POA) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge <ServantBase>& _b = ClientBase <T, ServantBase>::_bridge ();
	InterfaceDef_ptr _ret ((_b._epv ().epv.get_interface) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ServantBase>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <ServantBase>& _b = ClientBase <T, ServantBase>::_bridge ();
	Boolean _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge <ServantBase>& _b = ClientBase <T, ServantBase>::_bridge ();
	Boolean _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

#endif
