#ifndef NIRVANA_ORB_DYNAMICSERVANT_C_H_
#define NIRVANA_ORB_DYNAMICSERVANT_C_H_

#include "AbstractBase_c.h"

namespace CORBA {

typedef Nirvana::Interface Request; // Not defined yet
typedef Nirvana::T_ptr <Request> Request_ptr;
typedef Nirvana::T_var <Request> Request_var;
typedef Nirvana::T_out <Request> Request_out;
typedef Nirvana::T_inout <Request> Request_inout;

namespace Nirvana {

class DynamicServant;
typedef Nirvana::T_ptr <DynamicServant> DynamicServant_ptr;
typedef Nirvana::T_var <DynamicServant> DynamicServant_var;
typedef Nirvana::T_out <DynamicServant> DynamicServant_out;
typedef Nirvana::T_inout <DynamicServant> DynamicServant_inout;

template <>
class Bridge <DynamicServant> :
	public BridgeMarshal <DynamicServant>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BASE_STRUCT_ENTRY (CORBA::AbstractBase, CORBA_AbstractBase)
		}
		base;

		struct
		{
			void (*invoke) (Bridge <DynamicServant>*, BridgeMarshal <Request>*, EnvironmentBridge*);
			const Char* (*primary_interface) (Bridge <DynamicServant>*, EnvironmentBridge* env);
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
		BridgeMarshal <DynamicServant> (epv.interface)
	{}
};

template <class T>
class Client <T, DynamicServant> :
	public T
{
public:
	void _invoke (Request_ptr request);
	const Char* _primary_interface ();
};

template <class T>
void Client <T, DynamicServant>::_invoke (Request_ptr request)
{
	Environment _env;
	Bridge <DynamicServant>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.invoke) (&_b, request, &_env);
	_env.check ();
}

template <class T>
const Char* Client <T, DynamicServant>::_primary_interface ()
{
	Environment _env;
	Bridge <DynamicServant>& _b (T::_get_bridge (_env));
	const Char* _ret = (_b._epv ().epv.primary_interface) (&_b, &_env);
	_env.check ();
	return _ret;
}

class DynamicServant : public ClientInterface <DynamicServant, AbstractBase>
{};

}
}

#endif
