/*
~~~{.idl}
module CORBA {
module Nirvana {

/// \brief Interoperable Object Request.
///        Interface to the implementation of the some Inter-ORB protocol.
pseudo interface IORequest
{
	/// Returns a marshaler associated with this call.
	/// If the marshaler does not exist, it will be created.
	/// Caller mustn't duplicate this interface.
	readonly attribute Marshal marshaler;

	/// Return exception to caller.
	void exception (inout any e);

	/// Marks request as successful.
	/// If request procedure return without the explicit call of success (), request will
	/// return UNKNOWN exception to caller.
	void success ();
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_IOREQUEST_H_
#define NIRVANA_ORB_IOREQUEST_H_

#include "Marshal.h"

namespace CORBA {
namespace Nirvana {

class IORequest;
typedef I_ptr <IORequest> IORequest_ptr;
typedef I_var <IORequest> IORequest_var;
typedef I_out <IORequest> IORequest_out;


BRIDGE_BEGIN (IORequest, CORBA_NIRVANA_REPOSITORY_ID ("IORequest"))
Interface* (*marshaler) (Bridge <IORequest>*, EnvironmentBridge*);
void (*exception) (Bridge <IORequest>*, Type <Any>::ABI_inout);
void (*success) (Bridge <IORequest>*, EnvironmentBridge*);
BRIDGE_END ()


template <class T>
class Client <T, IORequest> :
	public T
{
public:
	Marshal_ptr marshaler ();
	void exception (Exception&& e) NIRVANA_NOEXCEPT;
	void success ();
};

class IORequest : public ClientInterface <IORequest>
{};

template <class T>
Marshal_ptr Client <T, IORequest>::marshaler ()
{
	Environment _env;
	Bridge <IORequest>& _b (T::_get_bridge (_env));
	I_VT_ret <Marshal> _ret = (_b._epv ().epv.marshaler) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::exception (Exception&& e) NIRVANA_NOEXCEPT
{
	Any any;
	any <<= (std::move (e));
	Environment* _env = nullptr;
	Bridge <IORequest>& _b (T::_get_bridge (*_env));
	(_b._epv ().epv.exception) (&_b, &Any_inout (any));
}

template <class T>
void Client <T, IORequest>::success ()
{
	Environment _env;
	Bridge <IORequest>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.success) (&_b, &_env);
	_env.check ();
}

}
}

#endif
