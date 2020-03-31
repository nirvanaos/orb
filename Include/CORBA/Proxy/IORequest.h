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
	readonly attribute Marshal marshaler;

	/// Return exception to caller.
	void exception (Exception e);

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
void (*system_exception) (Bridge <IORequest>*, Long, Type <String>::ABI_in, ULong, ABI_enum);
void (*user_exception) (Bridge <IORequest>*, Interface*, ::Nirvana::ConstPointer);
void (*success) (Bridge <IORequest>*, EnvironmentBridge*);
BRIDGE_END ()


template <class T>
class Client <T, IORequest> :
	public T
{
public:
	Marshal_ptr marshaler ();
	void exception (const Exception& e) NIRVANA_NOEXCEPT;
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
void Client <T, IORequest>::exception (const Exception& e) NIRVANA_NOEXCEPT
{
	Environment* _env = nullptr;
	Bridge <IORequest>& _b (T::_get_bridge (*_env));
	const SystemException* pse = SystemException::_downcast (&e);
	if (pse)
		(_b._epv ().epv.system_exception) (&_b, pse->__code (), &String_in (pse->_rep_id ()), pse->minor (), pse->completed ());
	else
		(_b._epv ().epv.user_exception) (&_b, &e.__type_code (), e.__data ());
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
