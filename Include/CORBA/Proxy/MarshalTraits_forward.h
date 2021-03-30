/// \file  MarshalTraits_forward.h
/// \brief Declares the MarshalTraits template.
/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_
#define NIRVANA_ORB_MARSHALTRAITS_FORWARD_H_

namespace CORBA {
namespace Nirvana {

/** For each structure, union or enum data type T, IDL compiler generates `CORBA::Nirvana::MarshalTraits <T>` structure.
    This structure is intended for proxies and defines how the parameters are marshalled inside the common platform domain.
    It is not intended for the user code.
~~~{.c++}
template <> struct MarshalTraits <T>
{
  /// \brief Copies input data to the marshaling buffer.
  /// \param src         Source value.
  /// \param marshaler   Marshaler interface. May be `nil` if MarshalTraits::has_marshal is `false`.
  /// \param dst         Destination value ABI.
  static void marshal_in (const T& src, Marshal_ptr marshaler, Type <T>::ABI_type <T>& dst);

  /// \brief Moves output data to the marshaling buffer.
  /// \param src         Source value. After marshalling, the value can be released.
  /// \param marshaler   Marshaler interface. May be `nil` if MarshalTraits::has_marshal is `false`.
  /// \param dst         Destination value ABI.
  static void marshal_out (T& src, Marshal_ptr marshaler, Type <T>::ABI_type <T>& dst);

  /// \brief Moves data from marshaling buffer to the local variable.
  /// \param src         Source value ABI.
  /// \param unmarshaler Unmarshaler interface. May be `nil` if MarshalTraits::has_marshal is `false`.
  /// \param dst         Destination value.
  static void unmarshal (const Type <T>::ABI_type <T>& src, Unmarshal_ptr unmarshaler, T& dst);

  /// \brief `true` if marshal operations are not trivial copy.
  static const bool has_marshal;

  /// \brief Serialize
  /// \param src         Source value ABI.
  /// \param stream      Output stream.
  static void serialize (const Type <T>::ABI_type <T>& src, Serialize_ptr stream);

  /// \brief Deserialize
  /// \param stream      Input stream.
  /// \param marshaler   Marshaler interface. May be `nil` if MarshalTraits::has_marshal is `false`.
  /// \param dst         Destination value ABI.
  static void deserialize (Deserialize_ptr stream, Marshal_ptr marshaler, Type <T>::ABI_type <T>& dst);
};
~~~
*/
template <class T> struct MarshalTraits;

}
}

#endif
