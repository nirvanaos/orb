#ifndef NIRVANA_ORB_ABSTRACTBASE_H_
#define NIRVANA_ORB_ABSTRACTBASE_H_

#include <Nirvana/NirvanaBase.h>
#include "Client.h"
#include "Type_interface.h"
#include "String.h"

namespace CORBA {

class AbstractBase;
typedef Nirvana::I_ptr <AbstractBase> AbstractBase_ptr;
typedef Nirvana::I_var <AbstractBase> AbstractBase_var;
typedef Nirvana::I_out <AbstractBase> AbstractBase_out;
typedef Nirvana::I_inout <AbstractBase> AbstractBase_inout;

class Object;
typedef Nirvana::I_ptr <Object> Object_ptr;
typedef Nirvana::I_var <Object> Object_var;
typedef Nirvana::I_out <Object> Object_out;
typedef Nirvana::I_inout <Object> Object_inout;

class ValueBase;

namespace Nirvana {

// AbstractBase

BRIDGE_BEGIN (AbstractBase, CORBA_REPOSITORY_ID ("AbstractBase"))
	Interface* (*query_interface) (Bridge <AbstractBase>*, ABI_in <String>, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, AbstractBase> :
	public T
{
public:
	Interface* _query_interface (String_in type_id);

	template <class I>
	I_ptr <I> _query_interface ()
	{
		return static_cast <I*> (_query_interface (Bridge <I>::interface_id_));
	}
};

template <class T>
Interface* Client <T, AbstractBase>::_query_interface (String_in type_id)
{
	Environment _env;
	Bridge <AbstractBase>& _b (T::_get_bridge (_env));
	Interface* _ret = (_b._epv ().epv.query_interface) (&_b, &type_id, &_env);
	_env.check ();
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
	static I_ptr <Primary> _narrow (AbstractBase_ptr obj)
	{
		return Primary::_duplicate (obj->_query_interface <Primary> ());
	}
};

}
}

#endif
