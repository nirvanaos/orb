#ifndef NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_
#define NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_

#include <Nirvana/SynchronizationContext.h>
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
  /// Move `out` (or returned) data from current domain memory to target domain memory.
  /// \param src Data in current domain memory.
  /// \param sc Target synchronization context.
  /// \param dst Data in the target domain memory.
  static void move_out (T& src, ::Nirvana::SynchronizationContext_ptr sc, T& dst);

  /// `true` if `move_out ()` method is not trivial.
  static const bool has_move_out;

  /// Copies `in` or `inout` data to local marshalling buffer.
  static void local_marshal (const T& src, T& dst);

  static void local_unmarshal_in (T& val);

  /// 'true` if `local_unmarshal_in()` is not empty.
  static const bool has_local_unmarshal_in;

  static void local_unmarshal_inout (T& val);

  /// 'true` if `local_unmarshal_inout()` is not empty.
  static const bool has_unmarshal_inout;
};
~~~
*/
template <class T> struct MarshalTraits;

extern const ::Nirvana::ImportInterfaceT <LocalMarshal> g_local_marshal;

}
}

#endif
