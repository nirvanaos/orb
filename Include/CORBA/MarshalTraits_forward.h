#ifndef NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_
#define NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_

#include <Nirvana/ImportInterface.h>
#include "LocalMarshal.h"

namespace CORBA {
namespace Nirvana {

/** For each structure, union or enum data type T, IDL compiler generates `CORBA::Nirvana::MarshalTraits <T>` structure.
    This structure is intended for proxies and defines how the parameters are marshalled.
    It is not intended for the user code.
~~~
template <> struct MarshalTraits <T>
{
  /// Copy `inout` data from the current domain memory to target domain memory
  /// \param src Source parameter.
  /// \param sc Destination synchronization context.
  /// \param dst Destination parameter.
  static void copy_inout (const T& src, SynchronizationContext_ptr sc, T& dst);

  /// Move `out` (or returned) data from target domain memory back to current domain memory.
  /// \param sc Source synchronization context.
  /// \param src Source data. Memory will be released.
  /// \param dst Destination parameter.
  static void move_out (SynchronizationContext_ptr sc, T&& src, T& dst);

  /// `true` if `copy_inout ()` and `move_out ()` methods are not trivial.
  static const bool has_copy_inout;

  /// Copies `in` or `inout` data to local marshalling buffer.
  static void local_marshal (const T& src, T& dst);

  static void local_unmarshal_in (T& val);

  static void local_unmarshal_inout (T& val);
};
~~~
*/
template <class T> struct MarshalTraits;

extern const ::Nirvana::ImportInterfaceT <LocalMarshal> g_local_marshal;

}
}

#endif
