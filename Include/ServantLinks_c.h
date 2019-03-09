#ifndef NIRVANA_ORB_SERVANTLINKS_C_H_
#define NIRVANA_ORB_SERVANTLINKS_C_H_

#include "ServantBase_c.h"

namespace CORBA {
namespace Nirvana {

class ServantLinks;
typedef T_ptr <ServantLinks> ServantLinks_ptr; // TODO: Value type, change to ServantLinks*
typedef T_var <ServantLinks> ServantLinks_var;
typedef T_out <ServantLinks> ServantLinks_out;

template <>
class Bridge <ServantLinks> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			ClientBridge <ServantBase>* (*servant_base) (Bridge <ServantLinks>*, EnvironmentBridge*);
			ClientBridge <Object>* (*object) (Bridge <ServantLinks>*, EnvironmentBridge*);
			Boolean (*is_active) (Bridge <ServantLinks>*, EnvironmentBridge*);
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
class ClientBase <T, ServantLinks>
{
public:
	operator ServantLinks& ()
	{
		Environment _env;
		T& t = static_cast <T&> (*this);
		Bridge <ServantLinks>* _ret = (t._epv ().base.CORBA_Nirvana_ServantLinks) (&t, Bridge <ServantLinks>::interface_id_, &_env);
		_env.check ();
		if (!_ret)
			throw MARSHAL ();
		return static_cast <ServantLinks&> (*_ret);
	}

	operator Bridge <ServantLinks>& ()
	{
		return operator ServantLinks& ();
	}
};

template <>
class ClientBase <ServantLinks, ServantLinks> :
	public ClientBridge <ServantLinks>
{};

template <class T>
class Client <T, ServantLinks> :
	public ClientBase <T, ServantLinks>
{
public:
	ServantBase_ptr servant_base ();
	ClientBridge <Object>* object ();
	Boolean is_active ();
};

template <class T>
ServantBase_ptr Client <T, ServantLinks>::servant_base ()
{
	Environment _env;
	Bridge <ServantLinks>& _b = (*this);
	// Do not release returned ptr, as it is value type. So we don't use ServantBase_var here.
	Bridge <ServantBase>* _ret = (_b._epv ().epv.servant_base) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
ClientBridge <Object>* Client <T, ServantLinks>::object ()
{
	Environment _env;
	Bridge <ServantLinks>& _b = (*this);
	ClientBridge <Object>* _ret = (_b._epv ().epv.object) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ServantLinks>::is_active ()
{
	Environment _env;
	Bridge <ServantLinks>& _b = (*this);
	Boolean _ret = (_b._epv ().epv.is_active) (&_b, &_env);
	_env.check ();
	return _ret;
}

class ServantLinks :
	public ClientInterfaceBase <ServantLinks>
{
public:
	typedef ServantLinks_ptr _ptr_type;
};

}
}

#endif
