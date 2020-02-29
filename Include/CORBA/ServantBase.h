// ServantBase interface.
#ifndef NIRVANA_ORB_SERVANTBASE_H_
#define NIRVANA_ORB_SERVANTBASE_H_

#include "POA_c.h"

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::PortableServer::ServantBase, PORTABLESERVER_REPOSITORY_ID (ServantBase))
BASE_STRUCT_ENTRY (AbstractBase, CORBA_AbstractBase)
BASE_STRUCT_ENTRY (Object, CORBA_Object)
BASE_STRUCT_ENTRY (ReferenceCounter, CORBA_Nirvana_ReferenceCounter)
BRIDGE_EPV
Interface* (*default_POA) (Bridge < ::PortableServer::ServantBase>*, EnvironmentBridge*);
Interface* (*get_interface) (Bridge < ::PortableServer::ServantBase>*, EnvironmentBridge*);
ABI_boolean (*is_a) (Bridge < ::PortableServer::ServantBase>*, ABI_in <String> type_id, EnvironmentBridge*);
ABI_boolean (*non_existent) (Bridge < ::PortableServer::ServantBase>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::PortableServer::ServantBase> :
	public T
{
public:
	::PortableServer::POA_ptr _default_POA ();
	InterfaceDef_ptr _get_interface ();
	Boolean _is_a (String_in type_id);
	Boolean _non_existent ();
};

template <class T>
::PortableServer::POA_ptr Client <T, ::PortableServer::ServantBase>::_default_POA ()
{
	Environment _env;
	Bridge <::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <::PortableServer::POA> _ret = (_b._epv ().epv.default_POA) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
InterfaceDef_ptr Client <T, ::PortableServer::ServantBase>::_get_interface ()
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	I_ret <InterfaceDef> _ret = (_b._epv ().epv.get_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::PortableServer::ServantBase>::_is_a (String_in type_id)
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.is_a) (&_b, &type_id, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::PortableServer::ServantBase>::_non_existent ()
{
	Environment _env;
	Bridge < ::PortableServer::ServantBase>& _b (T::_get_bridge (_env));
	T_ret <Boolean> _ret = (_b._epv ().epv.non_existent) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <> /// We can obtain I_ptr <ServantBase> directly from servant pointer
class I_ptr < ::PortableServer::ServantBase> : public I_ptr_base < ::PortableServer::ServantBase>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	/// We can obtain I_ptr directly from servant pointer
	I_ptr (Bridge < ::PortableServer::ServantBase>* p) NIRVANA_NOEXCEPT :
		I_ptr_base < ::PortableServer::ServantBase> (reinterpret_cast < ::PortableServer::ServantBase*> (p))
	{}

	I_ptr (const I_ptr& src) NIRVANA_NOEXCEPT :
		I_ptr_base (src)
	{}

	I_ptr (const I_var <::PortableServer::ServantBase>& var) NIRVANA_NOEXCEPT :
		I_ptr_base (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    Object_var func ();
	///    Object_ptr obj = func ();
	I_ptr (I_var <::PortableServer::ServantBase>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}

	I_ptr& operator = (const I_ptr& src) NIRVANA_NOEXCEPT
	{
		I_ptr_base::operator = (src);
		return *this;
	}

	/// When servant returns `I_ptr`, skeleton must be able to convert
	/// it to the ABI return type `Interface*`
	operator Bridge <::PortableServer::ServantBase>* () const NIRVANA_NOEXCEPT
	{
		assert (UNINITIALIZED_PTR != (uintptr_t)this->p_);
		return reinterpret_cast <Bridge <::PortableServer::ServantBase>*> (this->p_);
	}
};

}
}

namespace PortableServer {

typedef ::CORBA::Nirvana::I_ptr <ServantBase> Servant;

class ServantBase :
	public ::CORBA::Nirvana::ClientInterface <ServantBase>,
	// Client methods from bases AbstractBase and Object are not available directly on pointer to ServantBase.
	public ::CORBA::Nirvana::ClientBase <ServantBase, ::CORBA::Object>,
	public ::CORBA::Nirvana::ClientBase <ServantBase, ::CORBA::AbstractBase>,
	public ::CORBA::Nirvana::ClientBase <ServantBase, ::CORBA::Nirvana::ReferenceCounter>
{};

inline void release (Servant& ptr)
{
	::CORBA::release (ptr);
}

}

#endif
