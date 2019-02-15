// Nirvana project
// Object Request Broker
// PortableServer::POA interface
#ifndef NIRVANA_ORB_POA_C_H_
#define NIRVANA_ORB_POA_C_H_

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

struct ServantLinks
{
	Bridge <ServantBase>* servant_base;
	Bridge <Object>* object;
};

template <>
class Bridge < ::PortableServer::POA> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Object>* (*CORBA_Object) (Bridge < ::PortableServer::POA>*, EnvironmentBridge*);
		}
		base;

		struct
		{
			const Char* (*activate_object) (Bridge < ::PortableServer::POA>*, const ServantLinks*, EnvironmentBridge*);
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
class Client <T, ::PortableServer::POA> :
	public ClientBase <T, ::PortableServer::POA>
{
public:
	const Char* activate_object (const ServantLinks& servant);
};

template <class T>
const Char* Client <T, ::PortableServer::POA>::activate_object (const ServantLinks& servant)
{
	Environment _env;
	Bridge < ::PortableServer::POA>& _b = ClientBase <T, ::PortableServer::POA>::_bridge ();
	const Char* _ret ((_b._epv ().epv.activate_object) (&_b, &servant, &_env));
	_env.check ();
	return _ret;
}

}
}

namespace PortableServer {

class POA :
	public CORBA::Nirvana::ClientInterface <POA>,
	public CORBA::Nirvana::Client <POA, ::CORBA::Object>
{
public:
	typedef POA_ptr _ptr_type;

	operator ::CORBA::Object& ()
	{
		::CORBA::Environment _env;
		::CORBA::Object* _ret = static_cast <::CORBA::Object*> ((_epv ().base.CORBA_Object) (this, &_env));
		_env.check ();
		assert (_ret);
		return *_ret;
	}
};

}

#endif
