#ifndef NIRVANA_ORB_REFERENCECOUNTER_H_
#define NIRVANA_ORB_REFERENCECOUNTER_H_

#include "Client.h"
#include "TypeInterface.h"

namespace CORBA {
namespace Nirvana {

class ReferenceCounter;
typedef I_ptr <ReferenceCounter> ReferenceCounter_ptr;
typedef I_var <ReferenceCounter> ReferenceCounter_var;
typedef I_out <ReferenceCounter> ReferenceCounter_out;

template <>
class Bridge <ReferenceCounter> :
	public BridgeMarshal <ReferenceCounter>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*add_ref) (Bridge <ReferenceCounter>*, EnvironmentBridge*);
			void (*remove_ref) (Bridge <ReferenceCounter>*, EnvironmentBridge*);
			ULong (*refcount_value) (Bridge <ReferenceCounter>*, EnvironmentBridge*);
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
		BridgeMarshal <ReferenceCounter> (epv.interface)
	{}
};

template <class T>
class Client <T, ReferenceCounter> :
	public T
{
public:
	void _add_ref ();
	void _remove_ref ();
	ULong _refcount_value ();
};

class ReferenceCounter : public ClientInterface <ReferenceCounter>
{};

template <class T>
void Client <T, ReferenceCounter>::_add_ref ()
{
	Environment _env;
	Bridge <ReferenceCounter>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.add_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, ReferenceCounter>::_remove_ref ()
{
	Environment _env;
	Bridge <ReferenceCounter>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.remove_ref) (&_b, &_env);
	_env.check ();
}

template <class T>
ULong Client <T, ReferenceCounter>::_refcount_value ()
{
	Environment _env;
	Bridge <ReferenceCounter>& _b (T::_get_bridge (_env));
	ULong _ret = (_b._epv ().epv.refcount_value) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

#endif
