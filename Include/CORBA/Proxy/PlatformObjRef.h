/*
~~~{.idl}
module CORBA {
module Nirvana {

struct OperationIndex
{
	unsigned short interface_idx;
	unsigned short operation_idx;
};

/// Keeps reference to an object in the same platform domain.
pseudo interface PlatformObjRef : Object
{
	/// Creates PlatformMarshal object.
	PlatformMarshal create_marshaler ();

	/// Performs a synchronous call.
	/// \param op                Operation index.
	/// \param in_params         Input parameters block's pointer.
	///                          Contains input values for in and inout parameters.
	///                          May be `nil` if there aren't in and inout parameters.
	/// \param in_params_size    Size of the input parameters block.
	/// \param marshaler         `PlatformMarshal` object. May be `nil` on input. Always `nil` on output.
	/// \param out_params        Pointer to a memory buffer for output parameters.
	/// \param out_params_size   Size of the output parameters block.
	/// \returns                 `PlatformUnmarshal` object for returned data. May be `nil`.
	PlatformUnmarshal call (OperationIndex op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, [inout] PlatformMarshal marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size) raises (UnknownUserException);

	/// Performs an asynchronous call.
	/// \param op              Operation index.
	/// \param in_params       Input parameters block's pointer.
	///                        Contains input values for in and inout parameters.
	///                        May be nil if there aren't in and inout parameters.
	/// \param in_params_size  Size of the input parameters block.
	/// \param marshaler       PlatformMarshal object. May be nil.
	/// \param out_params_size Size of the output parameters block.
	/// \param _oneway         TRUE if this is oneway operation.
	/// \returns               AsyncCall object. Nil if _oneway is TRUE.
	AsyncCall async_call (OperationIndex op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, PlatformMarshal marshaler,
		::Nirvana::Size out_params_size, boolean _oneway);
};

}
}
~~~
*/
#ifndef NIRVANA_ORB_PLATFORMOBJREF_H_
#define NIRVANA_ORB_PLATFORMOBJREF_H_

#include "PlatformMarshal.h"
#include "PlatformUnmarshal.h"

namespace CORBA {
namespace Nirvana {

struct OperationIndex
{
	UShort interface_idx;
	UShort operation_idx;
};

template <> struct Type <OperationIndex> : TypeFixLen <OperationIndex> {};

class PlatformObjRef;
typedef I_ptr <PlatformObjRef> PlatformObjRef_ptr;
typedef I_var <PlatformObjRef> PlatformObjRef_var;
typedef I_out <PlatformObjRef> PlatformObjRef_out;

BRIDGE_BEGIN (PlatformObjRef, CORBA_NIRVANA_REPOSITORY_ID ("PlatformObjRef"))
BASE_STRUCT_ENTRY (CORBA::Object, CORBA_Object)
BRIDGE_EPV
Interface* (*create_marshaler) (Bridge <PlatformObjRef>*, EnvironmentBridge*);
Interface* (*call) (Bridge <PlatformObjRef>*, Type <OperationIndex>::ABI_in op,
	::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, Interface** marshaler,
	::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, PlatformObjRef> :
	public T
{
public:
	PlatformMarshal_ptr create_marshaler ();

	PlatformUnmarshal_ptr call (Type <OperationIndex>::C_in op,
		::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, TypeI <PlatformMarshal>::C_inout marshaler,
		::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size);
};

class PlatformObjRef : public ClientInterface <PlatformObjRef>
{};

template <class T>
PlatformMarshal_ptr Client <T, PlatformObjRef>::create_marshaler ()
{
	Environment _env;
	Bridge <PlatformObjRef>& _b (T::_get_bridge (_env));
	I_ret <PlatformMarshal> _ret ((_b._epv ().epv.create_marshaler) (&_b, &_env));
	_env.check ();
	return _ret;
}

template <class T>
PlatformUnmarshal_ptr Client <T, PlatformObjRef>::call (Type <OperationIndex>::C_in op,
	::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, TypeI <PlatformMarshal>::C_inout marshaler,
	::Nirvana::Pointer out_params, ::Nirvana::Size out_params_size)
{
	EnvironmentEx <UnknownUserException> _env;
	Bridge <PlatformObjRef>& _b (T::_get_bridge (_env));
	I_ret <PlatformUnmarshal> _ret ((_b._epv ().epv.call) (&_b, &op, in_params, in_params_size, &marshaler,
		out_params, out_params_size, &_env));
	_env.check ();
	return _ret;
}

}
}

#endif
