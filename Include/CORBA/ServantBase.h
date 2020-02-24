// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_H_
#define NIRVANA_ORB_SERVANTBASE_H_

#include "Object.h"

namespace PortableServer {

class POA;
typedef ::CORBA::Nirvana::I_ptr <POA> POA_ptr;
typedef ::CORBA::Nirvana::I_var <POA> POA_var;
typedef ::CORBA::Nirvana::I_out <POA> POA_out;

}

namespace CORBA {
namespace Nirvana {

class ServantBase;

template <> class I_ptr <ServantBase>;

typedef ::CORBA::Nirvana::I_ptr <ServantBase> ServantBase_ptr;
typedef ::CORBA::Nirvana::I_var <ServantBase> ServantBase_var;
typedef ::CORBA::Nirvana::I_out <ServantBase> ServantBase_out;

BRIDGE_BEGIN (ServantBase)
BASE_STRUCT_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Interface* (*default_POA) (Bridge <ServantBase>*, EnvironmentBridge*);
Interface* (*get_interface) (Bridge <ServantBase>*, EnvironmentBridge*);
ABI_boolean (*is_a) (Bridge <ServantBase>*, const Char* type_id, EnvironmentBridge*);
ABI_boolean (*non_existent) (Bridge <ServantBase>*, EnvironmentBridge*);
BRIDGE_END ()

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
	Bridge <ServantBase>& _b (T::_get_bridge (_env));
	I_ret <::PortableServer::POA> _ret = (_b._epv ().epv.default_POA) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge <ServantBase>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ServantBase>::_is_a (const Char* type_id)
{
	Environment _env;
	Bridge <ServantBase>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.is_a) (&_b, type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge <ServantBase>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

class ServantBase :
	public ClientInterface <ServantBase>,
	// Client methods from bases AbstractBase and Object are not available directly on pointer to ServantBase.
	public ClientBase <ServantBase, Object>,
	public ClientBase <ServantBase, AbstractBase>
{
public:
	static ServantBase_ptr _check (Interface* bridge);

	static const ::CORBA::Char check_interface_id_ [];
};

template <>
class I_ptr <ServantBase> :
	public I_ptr_base <ServantBase>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	I_ptr (Bridge <ServantBase>* servant) :
		I_ptr_base <ServantBase> (static_cast <ServantBase*> (servant))
	{}

	I_ptr (ServantBase* p) NIRVANA_NOEXCEPT :
		I_ptr_base <ServantBase> (p)
	{}

	I_ptr (const I_ptr <ServantBase>& src) NIRVANA_NOEXCEPT :
		I_ptr_base <ServantBase> (src)
	{}

	I_ptr (const I_var <ServantBase>& var) NIRVANA_NOEXCEPT :
		I_ptr_base <ServantBase> (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    ServantBase_var func ();
	///    ServantBase_ptr obj = func ();
	I_ptr (I_var <ServantBase>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}

	I_ptr& operator = (const I_ptr <ServantBase>& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base <ServantBase>::operator = (src);
		return *this;
	}

	/// When servant returns `I_ptr`, skeleton must be able to convert
	/// it to the ABI return type `Interface*`
	operator Bridge <ServantBase>* () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return static_cast <Bridge <ServantBase>*> (this->p_);
	}
};

inline ServantBase_ptr ServantBase::_check (Interface* bridge)
{
	return static_cast <ServantBase*> (Interface::_check (bridge, check_interface_id_));
}

}
}

#endif
