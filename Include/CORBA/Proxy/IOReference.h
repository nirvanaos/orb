/*
~~~{.idl}
module CORBA {
module Nirvana {

struct OperationIndex
{
	unsigned short interface_idx;
	unsigned short operation_idx;
};

/// \brief Interoperable Object Reference.
///        Interface to the implementation of the some Inter-ORB protocol.
pseudo interface IOReference : Object
{
	/// Creates Marshal object.
	Marshal create_marshaler ();

	/// Performs a synchronous call.
	/// \param op                Operation index.
	/// \param in_params         Input parameters block's pointer.
	///                          Contains input values for in and inout parameters.
	///                          May be `nil` if there aren't in and inout parameters.
	/// \param in_params_size    Size of the input parameters block.
	/// \param marshaler         `Marshal` object. May be `nil` on input. Always `nil` on output.
	/// \param out_params        Pointer to a memory buffer for output parameters.
	/// \param out_params_size   Size of the output parameters block.
	/// \returns                 `Unmarshal` object for returned data. May be `nil`.
	Unmarshal call (OperationIndex op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, [inout] Marshal marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size) raises (UnknownUserException);

	/// Performs an asynchronous call.
	/// \param op              Operation index.
	/// \param in_params       Input parameters block's pointer.
	///                        Contains input values for in and inout parameters.
	///                        May be nil if there aren't in and inout parameters.
	/// \param in_params_size  Size of the input parameters block.
	/// \param marshaler       Marshal object. May be nil.
	/// \param out_params_size Size of the output parameters block.
	/// \param _oneway         TRUE if this is oneway operation.
	/// \returns               AsyncCall object. Nil if _oneway is TRUE.
	AsyncCall async_call (OperationIndex op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Marshal marshaler,
		::Nirvana::Size out_params_size, boolean _oneway);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_IOREFERENCE_H_
#define NIRVANA_ORB_IOREFERENCE_H_

#include "../CORBA.h"
#include "Marshal.h"
#include "Unmarshal.h"

namespace CORBA {
namespace Nirvana {

struct OperationIndex
{
	UShort interface_idx;
	UShort operation_idx;
};

template <> struct Type <OperationIndex> : TypeFixLen <OperationIndex> {};

class IOReference;
typedef I_ptr <IOReference> IOReference_ptr;
typedef I_var <IOReference> IOReference_var;
typedef I_out <IOReference> IOReference_out;

BRIDGE_BEGIN (IOReference, CORBA_NIRVANA_REPOSITORY_ID ("IOReference"))
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Interface* (*create_marshaler) (Bridge <IOReference>*, EnvironmentBridge*);
Interface* (*call) (Bridge <IOReference>*, Type <OperationIndex>::ABI_in op,
	::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Interface** marshaler,
	::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, IOReference> :
	public T
{
public:
	Marshal_ptr create_marshaler ();

	Unmarshal_ptr call (Type <OperationIndex>::C_in op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, TypeI <Marshal>::C_inout marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size);
};

class IOReference : public ClientInterface <IOReference, Object>
{};

template <class T>
Marshal_ptr Client <T, IOReference>::create_marshaler ()
{
	Environment _env;
	Bridge <IOReference>& _b (T::_get_bridge (_env));
	I_ret <Marshal> _ret ((_b._epv ().epv.create_marshaler) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
Unmarshal_ptr Client <T, IOReference>::call (Type <OperationIndex>::C_in op,
	::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, TypeI <Marshal>::C_inout marshaler,
	::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size)
{
	EnvironmentEx <UnknownUserException> _env;
	Bridge <IOReference>& _b (T::_get_bridge (_env));
	I_ret <Unmarshal> _ret ((_b._epv ().epv.call) (&_b, &op, in_params, in_params_size, &marshaler,
		out_params, out_params_size, &_env));
	_env.check ();
	return _ret;
}

}
}

#endif
