/*
~~~{.idl}
module CORBA {
module Nirvana {

pseudo interface PlatformRequest
{
	/// Returns a marshaler associated with this call.
	/// If the marshaler does not exist, it will be created.
	PlatformMarshal marshaler ();

	void system_exception (long code, RepositoryId id, unsigned long minor, CompletionStatus completed);

	void user_exception (RepositoryId id, ::Nirvana::ConstPointer data, ::Nirvana::Size data_size);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_PLATFORMREQUEST_H_
#define NIRVANA_ORB_PLATFORMREQUEST_H_

#include "PlatformMarshal.h"

namespace CORBA {
namespace Nirvana {

class PlatformRequest;
typedef I_ptr <PlatformRequest> PlatformRequest_ptr;
typedef I_var <PlatformRequest> PlatformRequest_var;
typedef I_out <PlatformRequest> PlatformRequest_out;


BRIDGE_BEGIN (PlatformRequest, CORBA_NIRVANA_REPOSITORY_ID ("PlatformRequest"))
Interface* (*create_marshaler) (Bridge <PlatformRequest>*, EnvironmentBridge*);
void (*system_exception) (Bridge <PlatformMarshal>*, Long, Type <String>::ABI_in, ULong, ABI_enum, EnvironmentBridge*);
void (*user_exception) (Bridge <PlatformMarshal>*, Type <String>::ABI_in, ::Nirvana::ConstPointer, ::Nirvana::Size, EnvironmentBridge*);
BRIDGE_END ()


template <class T>
class Client <T, PlatformRequest> :
	public T
{
public:
	PlatformMarshal_var create_marshaler ();
	void system_exception (Long code, String_in id, ULong minor, CompletionStatus completed);
	void user_exception (String_in id, ::Nirvana::ConstPointer data, ::Nirvana::Size data_size);
};

template <class T>
PlatformMarshal_var Client <T, PlatformRequest>::create_marshaler ()
{
	Environment _env;
	Bridge <PlatformRequest>& _b (T::_get_bridge (_env));
	I_ret <PlatformMarshal> _ret = (_b._epv ().epv.create_marshaler) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, PlatformRequest>::system_exception (Long code, String_in id, ULong minor, CompletionStatus completed)
{
	Environment _env;
	Bridge <PlatformRequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.system_exception) (&_b, code, &id, minor, completed, &_env);
	_env.check ();
}

template <class T>
void Client <T, PlatformRequest>::user_exception (String_in id, ::Nirvana::ConstPointer data, ::Nirvana::Size data_size)
{
	Environment _env;
	Bridge <PlatformRequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.user_exception) (&_b, &id, data, data_size, &_env);
	_env.check ();
}

}
}

#endif
