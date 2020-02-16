// Nirvana project
// Object Request Broker
//! Client implementation templates.
#ifndef NIRVANA_ORB_CLIENT_H_
#define NIRVANA_ORB_CLIENT_H_

#include "I_ptr.h"
#include "Environment.h"
#include "RepositoryId.h"

namespace CORBA {
namespace Nirvana {

template <class T, class I> class Client
	: public T
{
	// Default implementation does not contain any methods.
};

template <class I>
class ClientBridge :
	public Bridge <I>
{
protected:
	Bridge <I>& _get_bridge (EnvironmentBase& env)
	{
		return *this;
	}
};

//! Primary interface client implementation.
template <class I>
class ClientInterfacePrimary :
	public Client <ClientBridge <I>, I>
{
public:
	typedef I_ptr <I> _ptr_type;

	static I_ptr <I> _duplicate (I_ptr <I> obj)
	{
		return static_cast <I*> (Interface::_duplicate (obj));
	}

	static I_ptr <I> _nil ()
	{
		return I_ptr <I>::nil ();
	}

	static I_ptr <I> _check (Interface* bridge)
	{
		return static_cast <I*> (Interface::_check (bridge, Bridge <I>::interface_id_));
	}
};

//! ClientBase - How base client obtains pointer to bridge from primary.
template <class Primary, class Base>
class ClientBase
{
public:
	operator I_ptr <Base> ();

protected:
	Bridge <Base>* _get_bridge_ptr (EnvironmentBase& env)
	{
		Primary& t = static_cast <Primary&> (*this);
		typename Bridge <Primary>:: template Wide <Base>::Func func = t._epv ().base;
		Bridge <Base>* ret = (func)(&t, Bridge <Base>::interface_id_, &env);
		env.check ();
		assert (!ret || RepositoryId::compatible (ret->_epv ().interface.interface_id, Bridge <Base>::interface_id_));
		return ret;
	}

	Bridge <Base>& _get_bridge (EnvironmentBase& env)
	{
		Bridge <Base>* ret = _get_bridge_ptr (env);
		if (!ret)
			::Nirvana::throw_INV_OBJREF ();
		return *ret;
	}
};

template <class Primary, class Base>
ClientBase <Primary, Base>::operator I_ptr <Base> ()
{
	Environment env;
	return static_cast <Base*> (_get_bridge_ptr (env));
}

//! Base interface client implementation.
//! Has specializations for Object and AbstractBase.
template <class Primary, class I>
class ClientInterfaceBase :
	public Client <ClientBase <Primary, I>, I>
{
};

//! Base for client interface.
template <class Primary, class ... Bases>
class ClientInterface :
	public ClientInterfacePrimary <Primary>,
	public ClientInterfaceBase <Primary, Bases>...
{};

}
}

#endif
