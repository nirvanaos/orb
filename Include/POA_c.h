// Nirvana project
// Object Request Broker
// PortableServer::POA interface
#ifndef NIRVANA_ORB_POA_C_H_
#define NIRVANA_ORB_POA_C_H_

#include "ServantLinks_c.h"

namespace CORBA {
namespace Nirvana {

class ServantBase;

template <>
class Bridge <POA> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge <POA>*, const Char*, EnvironmentBridge*);
		}
		base;

		struct
		{
			const Char* (*activate_object) (Bridge <POA>*, ClientBridge <ServantLinks>*, EnvironmentBridge*);
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
class ClientBase <T, POA>
{
public:
	operator POA& ()
	{
		Environment _env;
		T& t = static_cast <T&> (*this);
		Bridge <POA>* _ret = (t._epv ().base.CORBA_Nirvana_POA) (&t, Bridge <POA>::interface_id_, &_env);
		_env.check ();
		if (!_ret)
			throw MARSHAL ();
		return static_cast <POA&> (*_ret);
	}

	operator Bridge <POA>& ()
	{
		return operator  POA& ();
	}
};

template <>
class ClientBase <POA, POA> :
	public ClientBridge <POA>
{};

template <class T>
class Client <T, POA> :
	public ClientBase <T, POA>
{
public:
	const Char* activate_object (ServantLinks_ptr servant);
};

template <class T>
const Char* Client <T, POA>::activate_object (ServantLinks_ptr servant)
{
	Environment _env;
	Bridge <POA>& _b = (*this);
	const Char* _ret = (_b._epv ().epv.activate_object) (&_b, servant, &_env);
	_env.check ();
	return _ret;
}

class POA :
	public ::CORBA::Nirvana::ClientInterface <POA>,
	public ::CORBA::Nirvana::Client <POA, ::CORBA::Object>
{
public:
	typedef POA_ptr _ptr_type;

};

}
}

#endif
