#ifndef NIRVANA_ORB_BINDER_C_H_
#define NIRVANA_ORB_BINDER_C_H_

#include "Interface_c.h"
#include "String.h"

namespace CORBA {
namespace Nirvana {

class Binder;
typedef T_ptr <Binder> Binder_ptr;
typedef T_var <Binder> Binder_var;
typedef T_out <Binder> Binder_out;

template <>
class Bridge <Binder> :
	public BridgeMarshal <Binder>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Interface>* (*bind) (Bridge <Binder>*, const ::CORBA::String_in*, const ::CORBA::String_in*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <Binder> (epv.interface)
	{}
};

template <class T>
class Client <T, Binder> :
	public T
{
public:
	Bridge <Interface>* bind (const ::CORBA::String_in& name, const ::CORBA::String_in& interface_id);
};

template <class T>
Bridge <Interface>* Client <T, Binder>::bind (const ::CORBA::String_in& name, const ::CORBA::String_in& interface_id)
{
	Environment _env;
	Bridge <Binder>& _b (T::_get_bridge (_env));
	Bridge <Interface>* _ret = (_b._epv ().epv.bind) (&_b, &name, &interface_id, &_env);
	_env.check ();
	return _ret;
}

class Binder : public ClientInterface <Binder>
{};

}
}

#endif

