// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_C_H_
#define NIRVANA_ORB_SERVANTBASE_C_H_

#include "Object_c.h"

namespace CORBA {
namespace Nirvana {

class POA;
typedef T_ptr <POA> POA_ptr;
typedef T_var <POA> POA_var;
typedef T_out <POA> POA_out;

class ServantBase;
typedef T_ptr <ServantBase> ServantBase_ptr;
typedef T_var <ServantBase> ServantBase_var;
typedef T_out <ServantBase> ServantBase_out;

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
			Bridge <AbstractBase>* (*CORBA_AbstractBase) (Bridge <ServantBase>*, const Char*, EnvironmentBridge*);
		}
		base;

		struct
		{
			ClientBridge <POA>* (*default_POA) (Bridge <ServantBase>*, EnvironmentBridge*);
			ClientBridge <InterfaceDef>* (*get_interface) (Bridge <ServantBase>*, EnvironmentBridge*);
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
		Bridge <Interface> (epv.interface)
	{}
};

template <class T>
class ClientBase <T, ServantBase>
{
public:
	operator ServantBase& ()
	{
		Environment _env;
		T& t = static_cast <T&> (*this);
		Bridge <ServantBase>* _ret = (t._epv ().base.CORBA_Nirvana_ServantBase) (&t, Bridge <ServantBase>::interface_id_, &_env);
		_env.check ();
		if (!_ret)
			throw MARSHAL ();
		return static_cast <ServantBase&> (*_ret);
	}

	operator Bridge <ServantBase>& ()
	{
		return operator ServantBase& ();
	}
};

template <>
class ClientBase <ServantBase, ServantBase> :
	public ClientBridge <ServantBase>
{};

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

template <class T>
POA_ptr Client <T, ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <ServantBase>& _b (*this);
	POA_var _ret ((_b._epv ().epv.default_POA) (&_b, &_env));
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
	public ::CORBA::Nirvana::Client <ServantBase, ::CORBA::AbstractBase>
{
public:
	typedef ServantBase_ptr _ptr_type;
};

}
}

#endif
