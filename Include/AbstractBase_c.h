#ifndef NIRVANA_ORB_ABSTRACTBASE_C_H_
#define NIRVANA_ORB_ABSTRACTBASE_C_H_

#include "Interface_c.h"

namespace CORBA {

typedef Nirvana::T_ptr <AbstractBase> AbstractBase_ptr;
typedef Nirvana::T_var <AbstractBase> AbstractBase_var;
typedef Nirvana::T_out <AbstractBase> AbstractBase_out;

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

protected:
	Bridge (const EPV& epv) :
		Bridge <Interface> (epv.interface)
	{}

	Bridge ()
	{}
};

template <class T>
class Client <T, AbstractBase> :
	public ClientBase <T, AbstractBase>
{
public:
	Bridge <Interface>* _find_interface (const Char* type_id);
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
	public Nirvana::ClientInterface <AbstractBase>
{
public:
	typedef AbstractBase_ptr _ptr_type;

	inline Object_ptr _to_object ();
};

}

#endif
