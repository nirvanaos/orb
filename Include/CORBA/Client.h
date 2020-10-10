// Nirvana project
// Object Request Broker
//! Client implementation templates.
#ifndef NIRVANA_ORB_CLIENT_H_
#define NIRVANA_ORB_CLIENT_H_

#include "Environment.h"
#include "Type_interface.h"

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
	Bridge <I>& _get_bridge (EnvironmentBase& env) NIRVANA_NOEXCEPT
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
	typedef I_var <I> _var_type;
	typedef I_out <I> _out_type;
	typedef I_inout <I> _inout_type;

	// TODO: Change return type to I_var?
	static NIRVANA_NODISCARD I_ptr <I> _duplicate (I_ptr <I> obj)
	{
		return static_cast <I*> (interface_duplicate (&obj));
	}

	static I_ptr <I> _nil ()
	{
		return I_ptr <I> ((I*)nullptr);
	}

	static I_ptr <I> _check (Interface* bridge)
	{
		return static_cast <I*> (Interface::_check (bridge, Bridge <I>::repository_id_));
	}

	operator I_ptr <Interface> ()
	{
		return this;
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
		Bridge <Base>* ret = (func)(&t, Bridge <Base>::repository_id_, &env);
		env.check ();
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
