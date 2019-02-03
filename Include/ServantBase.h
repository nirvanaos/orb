// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_H_
#define NIRVANA_ORB_SERVANTBASE_H_

#include "Object.h"

namespace PortableServer {

class POA;
//typedef CORBA::Nirvana::T_ptr <POA> POA_ptr;
typedef CORBA::Nirvana::Bridge <POA>* POA_ptr; // Not defined yet
}

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
/* ??? Is this necessary?
	static const Char* _primary_interface ()
	{
		return NIRVANA_REPOSITORY_ID (ServantBase);
	}
*/
protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
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
	public ClientInterface <ServantBase>,
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

// ServantBase skeleton
template <class S>
class Skeleton <S, ServantBase>
{
public:
	static const typename Bridge <ServantBase>::EPV epv_;

protected:
	static Bridge <POA>* __default_POA (Bridge <ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._default_POA ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Bridge <InterfaceDef>* __get_interface (Bridge <ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._get_interface ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __is_a (Bridge <ServantBase>* obj, const Char* type_id, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._is_a (type_id);
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}

	static Boolean __non_existent (Bridge <ServantBase>* obj, EnvironmentBridge* env)
	{
		try {
			return S::_implementation (obj)._non_existent ();
		} catch (const Exception& e) {
			env->set_exception (e);
		} catch (...) {
			env->set_unknown_exception ();
		}
		return 0;
	}
};

template <class S>
const Bridge <ServantBase>::EPV Skeleton <S, ServantBase>::epv_ = {
	{ // interface
		S::template _duplicate <ServantBase>,
		S::template _release <ServantBase>
	},
	{ // base
		S::template _wide <::CORBA::AbstractBase, ServantBase>
	},
	{ // epv
		S::__default_POA,
		S::__get_interface,
		S::__is_a,
		S::__non_existent
	}
};

}
}

#endif
