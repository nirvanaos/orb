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
pseudo interface PlatformObjRef
{
  /// Returns Object proxy.
  /// \param iid Repository id of the Object interface.
  Object get_object (RepositoryId iid);

  /// Creates PlatformMarshal object.
  PlatformMarshal create_marshaler ();

  /// Performs a synchronous call.
  /// \param op                Operation index.
  /// \param in_params         Input parameters block's pointer. 
  ///                          Contains input values for in and inout parameters.
  ///                          May be `nil` if there aren't in and inout parameters.
  /// \param in_params_size    Size of the input parameters block.
  /// \param marshaler         `PlatformMarshal` object. May be `nil`.
  /// \param out_params        Pointer to a memory buffer for output parameters.
  /// \param out_params_size   Size of the output parameters block.
  /// \returns                 `PlatformUnmarshal` object for returned data. May be `nil`.
  PlatformUnmarshal call (OperationIndex op, 
    ::Nirvana::ConstPointer in_params, ::Nirvana::Size in_params_size, PlatformMarshal marshaler,
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

class PlatformObjRef;
typedef I_ptr <PlatformObjRef> PlatformObjRef_ptr;
typedef I_var <PlatformObjRef> PlatformObjRef_var;
typedef I_out <PlatformObjRef> PlatformObjRef_out;

}
}

#endif
