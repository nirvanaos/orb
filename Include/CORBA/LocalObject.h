// CORBA::LocalObject class for compatibility with C++ mapping standard.
// Rarely used.
#ifndef NIRVANA_ORB_LOCALOBJECT_H_
#define NIRVANA_ORB_LOCALOBJECT_H_

#include "ImplementationPOA.h"

namespace CORBA {

class LocalObject;

namespace Nirvana {

template <>
class I_ptr <CORBA::LocalObject> :
	public I_ptr_base <CORBA::LocalObject>
{
public:
	I_ptr () NIRVANA_NOEXCEPT
	{}

	I_ptr (ServantPOA <Object>* servant) NIRVANA_NOEXCEPT;

	I_ptr (const I_ptr& src) NIRVANA_NOEXCEPT :
		I_ptr_base <CORBA::LocalObject> (src)
	{}

	I_ptr (const I_var <CORBA::LocalObject>& var) NIRVANA_NOEXCEPT :
		I_ptr_base <CORBA::LocalObject> (var)
	{}

	/// Move constructor in case returned I_var assigned to I_ptr:
	///    ServantBase_var func ();
	///    ServantBase_ptr obj = func ();
	I_ptr (I_var <CORBA::LocalObject>&& var) NIRVANA_NOEXCEPT
	{
		this->move_from (var);
	}
};

class LocalObjectBase :
	public Nirvana::ServantPOA <Object>
{
public:
	Bridge <Object>& _get_bridge (EnvironmentBase&)
	{
		return *static_cast <Bridge <Object>*> (LocalObjectLink::_get_proxy (Object::interface_id_));
	}

	operator Object_ptr ()
	{
		return static_cast <Object*> (LocalObjectLink::_get_proxy (Object::interface_id_));
	}
};

}

typedef Nirvana::I_ptr <LocalObject> LocalObject_ptr;
typedef PortableServer::Servant_var <LocalObject> LocalObject_var;

class LocalObject :
	public Nirvana::Client <Nirvana::LocalObjectBase, Object>
{
public:
	typedef LocalObject_ptr _ptr_type;
	typedef PortableServer::Servant_var <LocalObject> _var_type;

	static LocalObject_ptr _duplicate (LocalObject_ptr obj)
	{
		obj->_add_ref ();
		return obj;
	}

	static LocalObject_ptr _nil ()
	{
		return LocalObject_ptr (0);
	}
};

inline void release (LocalObject_ptr obj)
{
	if (obj)
		obj->_remove_ref ();
}

namespace Nirvana {

inline
I_ptr <CORBA::LocalObject>::I_ptr (ServantPOA <Object>* servant) NIRVANA_NOEXCEPT :
I_ptr_base (static_cast <CORBA::LocalObject*> (servant))
{}

}
}

#endif
