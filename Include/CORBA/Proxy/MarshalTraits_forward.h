/// \file  MarshalTraits_forward.h
/// \brief Declares the MarshalTraits template.
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
  /// \brief Copies input data to the marshaling buffer.
  /// \param src         Source value.
  /// \param marshaler   Marshaler interface. May be `nil` if MarshalTraits::has_marshal is `false`.
  /// \param dst         Destination value ABI.
  static void marshal_in (const T& src, PlatformMarshal_ptr marshaler, Type <T>::ABI_type <T>& dst);

  /// Moves output data to the marshaling buffer.
  /// \param src         Source value. After marshalling, the value can be released.
  /// \param marshaler   Marshaler interface. May be `nil` if MarshalTraits::has_marshal is `false`.
  /// \param dst         Destination value ABI.
  static void marshal_out (T& src, PlatformMarshal_ptr marshaler, Type <T>::ABI_type <T>& dst);

  /// Moves data from marshaling buffer to the local variable.
  /// \param src         Source value ABI.
  /// \param unmarshaler Unmarshaler interface. May be `nil` if MarshalTraits::has_marshal is `false`.
  /// \param dst         Destination value.
  static void unmarshal (Type <T>::ABI_type <T>& src, PlatformUnmarshal_ptr unmarshaler, T& dst);

  /// `true` if marshal operations are not trivial copy.
  static const bool has_marshal;
};
~~~
*/
template <class T> struct MarshalTraits;

}
}

#endif
