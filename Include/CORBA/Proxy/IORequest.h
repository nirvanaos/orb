/*
~~~{.idl}
module CORBA {
module Nirvana {

/// \brief Interface to the implementation of the some Inter-ORB protocol.
pseudo interface IORequest
{
	/// Returns a marshaler associated with this call.
	/// If the marshaler does not exist, it will be created.
	readonly attribute Marshal marshaler;

	void system_exception (long code, RepositoryId id, unsigned long minor, CompletionStatus completed);

	void user_exception (TypeCode tc, ::Nirvana::ConstPointer data);

	void unknown_exception ();
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
void (*unknown_exception) (Bridge <IORequest>*);
BRIDGE_END ()


template <class T>
class Client <T, IORequest> :
	public T
{
public:
	Marshal_var marshaler ();

	void system_exception (
		Long code, String_in id, ULong minor, CompletionStatus completed) NIRVANA_NOEXCEPT;
	void system_exception (const SystemException& e) NIRVANA_NOEXCEPT;

	void user_exception (I_in <TypeCode> tc, ::Nirvana::ConstPointer data) NIRVANA_NOEXCEPT;
	void user_exception (const UserException& e) NIRVANA_NOEXCEPT;

	void unknown_exception () NIRVANA_NOEXCEPT;
};

class IORequest : public ClientInterface <IORequest>
{};

template <class T>
Marshal_var Client <T, IORequest>::marshaler ()
{
	Environment _env;
	Bridge <IORequest>& _b (T::_get_bridge (_env));
	I_ret <Marshal> _ret = (_b._epv ().epv.marshaler) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, IORequest>::system_exception (
	Long code, String_in id, ULong minor, CompletionStatus completed) NIRVANA_NOEXCEPT
{
	Environment* _env = nullptr;
	Bridge <IORequest>& _b (T::_get_bridge (*_env));
	(_b._epv ().epv.system_exception) (&_b, code, &id, minor, completed);
}

template <class T>
void Client <T, IORequest>::user_exception (
	I_in <TypeCode> tc, ::Nirvana::ConstPointer data) NIRVANA_NOEXCEPT
{
	Environment* _env = nullptr;
	Bridge <IORequest>& _b (T::_get_bridge (*_env));
	(_b._epv ().epv.user_exception) (&_b, &tc, data);
}

template <class T>
void Client <T, IORequest>::unknown_exception () NIRVANA_NOEXCEPT
{
	Environment* _env = nullptr;
	Bridge <IORequest>& _b (T::_get_bridge (*_env));
	(_b._epv ().epv.unknown_exception) (&_b);
}

template <class T>
void Client <T, IORequest>::system_exception (const SystemException& e) NIRVANA_NOEXCEPT
{
	system_exception (e.__code (), e._rep_id (), e.minor (), e.completed ());
}

template <class T>
void Client <T, IORequest>::user_exception (const UserException& e) NIRVANA_NOEXCEPT
{
	user_exception (e.__type_code (), e.__data ());
}

}
}

#endif
