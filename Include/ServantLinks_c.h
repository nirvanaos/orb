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
	public BridgeMarshal <ServantLinks>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BridgeMarshal <ServantBase>* (*servant_base) (Bridge <ServantLinks>*, EnvironmentBridge*);
			BridgeMarshal <Object>* (*object) (Bridge <ServantLinks>*, EnvironmentBridge*);
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
		BridgeMarshal <ServantLinks> (epv.interface)
	{}
};

template <class T>
class Client <T, ServantLinks> :
	public T
{
public:
	ServantBase_ptr servant_base ();
	Object_ptr object ();
	Boolean is_active ();
};

template <class T>
ServantBase_ptr Client <T, ServantLinks>::servant_base ()
{
	Environment _env;
	Bridge <ServantLinks>& _b = (*this);
	// Do not release returned ptr, as it is value type. So we don't use ServantBase_var here.
	BridgeMarshal <ServantBase>* _ret = (_b._epv ().epv.servant_base) (&_b, &_env);
	_env.check ();
	return static_cast <ServantBase*> (_ret); // No adoption needed.
}

template <class T>
Object_ptr Client <T, ServantLinks>::object ()
{
	Environment _env;
	Bridge <ServantLinks>& _b = (*this);
	BridgeMarshal <Object>* _ret = (_b._epv ().epv.object) (&_b, &_env);
	_env.check ();
	return static_cast <Object*> (_ret); // No adoption needed.
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
	public ClientInterfacePrimary <ServantLinks>
{
public:
	static ServantLinks_ptr unmarshal (BridgeMarshal <ServantLinks>* bridge)
	{
		assert (bridge);
		ServantLinks* p = static_cast <ServantLinks*> (Interface::unmarshal (bridge, Bridge <ServantLinks>::interface_id_));
		Interface::unmarshal (p->object (), Bridge <Object>::interface_id_);
		Interface::unmarshal (p->servant_base (), Bridge <ServantBase>::interface_id_);
		return p;
	}
};

}
}

#endif
