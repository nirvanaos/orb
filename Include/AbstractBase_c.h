#ifndef NIRVANA_ORB_ABSTRACTBASE_C_H_
#define NIRVANA_ORB_ABSTRACTBASE_C_H_

#include "Interface_c.h"
#include "Environment.h"

namespace CORBA {

class AbstractBase;
typedef Nirvana::T_ptr <AbstractBase> AbstractBase_ptr;
typedef Nirvana::T_var <AbstractBase> AbstractBase_var;
typedef Nirvana::T_out <AbstractBase> AbstractBase_out;

class Object;
typedef Nirvana::T_ptr <Object> Object_ptr;
typedef Nirvana::T_var <Object> Object_var;
typedef Nirvana::T_out <Object> Object_out;

class ValueBase;

namespace Nirvana {

// AbstractBase

template <>
class Bridge <AbstractBase> :
	public Bridge <Interface>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Interface>* (*find_interface) (Bridge <AbstractBase>*, const Char*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (const EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}
};

template <class T>
class Client <T, AbstractBase> :
	public ClientBase <T, AbstractBase>
{
protected:
	template <class I> friend class ClientInterface;

	Bridge <Interface>* _find_interface (const Char* type_id);

	template <class I>
	T_ptr <I> _find_interface ()
	{
		return static_cast <I*> (_find_interface (Bridge <I>::interface_id_));
	}
};

template <class T>
Bridge <Interface>* Client <T, AbstractBase>::_find_interface (const Char* type_id)
{
	Environment env;
	Bridge <AbstractBase>& bridge = ClientBase <T, AbstractBase>::_bridge ();
	Bridge <Interface>* ret = (bridge._epv ().epv.find_interface) (&bridge, type_id, &env);
	env.check ();
	return ret;
}

}

class AbstractBase :
	public Nirvana::ClientInterfaceBase <AbstractBase>
{
public:
	typedef AbstractBase_ptr _ptr_type;

	static AbstractBase_ptr _narrow (AbstractBase_ptr obj)
	{
		return _duplicate (obj);
	}

	inline Object_ptr _to_object ();
	inline ValueBase* _to_value ();
};

namespace Nirvana {

template <class I>
class ClientInterfacePseudo :
	public ClientInterfaceBase <I>
{
public:
	static T_ptr <I> _narrow (AbstractBase_ptr obj)
	{
		return obj->_find_interface <I> ();
	}
};

}

}

#endif
