#ifndef NIRVANA_ORB_ABSTRACTBASE_C_H_
#define NIRVANA_ORB_ABSTRACTBASE_C_H_

#include "Interface_c.h"

namespace CORBA {

class AbstractBase;
typedef Nirvana::T_ptr <AbstractBase> AbstractBase_ptr;
typedef Nirvana::T_var <AbstractBase> AbstractBase_var;
typedef Nirvana::T_out <AbstractBase> AbstractBase_out;
typedef Nirvana::T_inout <AbstractBase> AbstractBase_inout;

class Object;
typedef Nirvana::T_ptr <Object> Object_ptr;
typedef Nirvana::T_var <Object> Object_var;
typedef Nirvana::T_out <Object> Object_out;
typedef Nirvana::T_inout <Object> Object_inout;

class ValueBase;

namespace Nirvana {

// AbstractBase

template <>
class Bridge <AbstractBase> :
	public BridgeMarshal <AbstractBase>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Interface>* (*query_interface) (Bridge <AbstractBase>*, const Char*, EnvironmentBridge*);
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
		BridgeMarshal <AbstractBase> (epv.interface)
	{}
};

template <class T>
class Client <T, AbstractBase> :
	public T
{
public:
	Bridge <Interface>* _query_interface (const Char* type_id);

	template <class I>
	T_ptr <I> _query_interface ()
	{
		return static_cast <I*> (_query_interface (Bridge <I>::interface_id_));
	}
};

template <class T>
Bridge <Interface>* Client <T, AbstractBase>::_query_interface (const Char* type_id)
{
	Environment _env;
	Bridge <AbstractBase>& _b (T::_get_bridge (_env));
	Bridge <Interface>* _ret = (_b._epv ().epv.query_interface) (&_b, type_id, &_env);
	_env.check ();
	assert (!_ret || RepositoryId::compatible (_ret->_epv ().interface_id, type_id));
	return _ret;
}

}

class AbstractBase :
	public Nirvana::ClientInterfacePrimary <AbstractBase>
{
public:
	static AbstractBase_ptr _narrow (AbstractBase_ptr obj)
	{
		return _duplicate (obj);
	}

	inline Object_ptr _to_object ();
	inline ValueBase* _to_value ();
};

namespace Nirvana {

template <class Primary>
class ClientInterfaceBase <Primary, AbstractBase> :
	public Client <ClientBase <Primary, AbstractBase>, AbstractBase>
{
public:
	static T_ptr <Primary> _narrow (AbstractBase_ptr obj)
	{
		return obj->_query_interface <Primary> ();
	}
};

}
}

#endif
