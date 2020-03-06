#ifndef NIRVANA_ORB_MARSHALTRAITS_STRING_H_
#define NIRVANA_ORB_MARSHALTRAITS_STRING_H_

#include "MarshalTraits_forward.h"
#include "String.h"

namespace CORBA {
namespace Nirvana {

template <typename C>
struct MarshalTraits <StringT <C> >
{
  static const bool has_move_out = true;
  static const bool has_unmarshal_in_ = false;
  static const bool has_unmarshal_inout_ = true;
 
  typedef ABI <StringT <C> > ABI;

  static bool _small_copy (const StringT <C>& src, StringT <C>& dst);
 
  static void move_out (StringT <C>& src, ::Nirvana::SynchronizationContext_ptr sc, StringT <C>& dst)
  {
    assert (dst.empty ());
    if (!_small_copy (src, dst)) {
      size_t cb = src.allocated ();
      dst.large_pointer ((C*)sc->adopt_output (src.large_pointer (), cb));
      dst.allocated (cb);
      dst.large_size (src.large_size ());
      src.reset ();
    }
  }

  static void local_marshal (const StringT <C>& src, StringT <C>& dst)
  {
    if (!_small_copy (src, dst)) {
      size_t size = src.large_size ();
      size_t cb = StringT <C>::byte_size (size);
      dst.large_pointer ((C*)CORBA::Nirvana::g_local_marshal->marshal_memory (src.large_pointer (), cb));
      dst.large_size (size);
      dst.allocated (cb);
    }
  }

  static void local_unmarshal_in (StringT <C>& val) NIRVANA_NOEXCEPT
  {}

  static void local_unmarshal_inout (StringT <C>& val) NIRVANA_NOEXCEPT
  {
    if (val.is_large ()) {
      size_t cb = val.allocated ();
      if (cb)
        CORBA::Nirvana::g_local_marshal->adopt_memory (val.large_pointer (), cb);
      else
        val.assign_internal (val.large_size (), val.large_pointer ());
    }
  }
};

template <typename C>
bool MarshalTraits <StringT <C> >::_small_copy (const StringT <C>& src, StringT <C>& dst)
{
  const C* p;
  size_t len;
  if (src.is_large ()) {
    len = src.large_size ();
    if (len > ABI::SMALL_CAPACITY)
      return false;
    p = src.large_pointer ();
  } else {
    p = src.small_pointer ();
    len = src.small_size ();
  }
  ::Nirvana::real_copy (p, p + len + 1, dst.small_pointer ());
  dst.small_size (len);
  return true;
}

}
}

#endif
