#ifndef NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_
#define NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_

namespace CORBA {
namespace Nirvana {

/** For each structure, union or enum data type T, IDL compiler generates `CORBA::Nirvana::MarshalTraits <T>` structure.
    This structure is intended for proxies and defines how the parameters are marshalled inside one platform domain.
    It is not intended for the user code.
~~~{.c++}
template <> struct MarshalTraits <T>
{
  /// Copies input data to marshalling buffer.
  static void marshal_in (const T& src, PlatformMarshal_ptr marshaler, Type <T>::ABI_type <T>& dst);

  /// Copies output data to marshalling buffer.
  /// \param src Source value. After marshalling, the value can be released.
  static void marshal_out (T& src, PlatformMarshal_ptr marshaler, Type <T>::ABI_type <T>& dst);

  static void unmarshal (Type <T>::ABI_type <T>& src, PlatformUnmarshal_ptr unmarshaler, T& dst);

  /// 'true` if `unmarshal_in()` is not simple copy in the specified context.
  static bool has_unmarshal (PlatformMarshalContext mctx);
};
~~~
*/
template <class T> struct MarshalTraits;

}
}

#endif
