#ifndef NIRVANA_ORB_ABSTRACTBASE_C_H_
#define NIRVANA_ORB_ABSTRACTBASE_C_H_

#include "Interface_c.h"
#include "Environment.h"

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
	public Bridge <Interface>
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
		Bridge <Interface> (epv.interface)
	{}
};

template <class T>
class ClientBase <T, AbstractBase>
{
public:
	operator AbstractBase& ()
	{
		Environment _env;
		T& t = static_cast <T&> (*this);
		Bridge <AbstractBase>* _ret = (t._epv ().base.CORBA_AbstractBase) (&t, Bridge <AbstractBase>::interface_id_, &_env);
		_env.check ();
		if (!_ret)
			throw MARSHAL ();
		return static_cast <AbstractBase&> (*_ret);
	}

	operator Bridge <AbstractBase>& ()
	{
		return operator AbstractBase& ();
	}
};

template <class T>
class Client <T, AbstractBase> :
	public T
{
protected:
	template <class I> friend class ClientInterface; // TODO: Does it really need?

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
	Environment env;
	Bridge <AbstractBase>& _b (*this);
	Bridge <Interface>* ret = (_b._epv ().epv.query_interface) (&_b, type_id, &env);
	env.check ();
	return ret;
}

}

class AbstractBase :
	public Nirvana::ClientInterfacePrimary <AbstractBase>
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

//! \class	ClientInterfacePseudo
//!
//! \brief	A pseudo interface derived from `AbstractBase'.
//!
//! \tparam	I	Interface.

template <class I>
class ClientInterfacePseudo :
	public ClientInterfacePrimary <I>
{
public:
	static T_ptr <I> _narrow (AbstractBase_ptr obj)
	{
		return obj->_query_interface <I> ();
	}
};

}

}

#endif
