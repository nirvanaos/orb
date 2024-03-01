/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
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
#ifndef NIRVANA_ORB_TYPE_FORWARD_H_
#define NIRVANA_ORB_TYPE_FORWARD_H_
#pragma once

namespace CORBA {
namespace Internal {

/** For each structure, union or enum data type T, IDL compiler generates `CORBA::Internal::Type <T>` structure.
    This structure defines how the parameters are passed between client and server.
~~~
template <> struct Type <T>
{
  typedef T Var;
  typedef Var VRet;
  typedef const T& ConstRef;
  typedef ABI <T> ABI;

  /// Check internal invariants and throw BAD_PARAM or INV_OBJREF exception if data is invalid.
  static void check (const ABI&);

  /// `true` if check () method is not empty.
  ///
  /// has_check = `true` for:
  /// - Variable-length data types
  /// - enum
  /// - char
  static const bool has_check;

  /// `true` for variable-length data types.
  /// A type is variable-length if it is one of the following types:
  /// - The type any
  /// - A bounded or unbounded string or wide string
  /// - A bounded or unbounded sequence
  /// - An object reference or reference to a transmissible pseudo-object
  /// - A valuetype
  /// - A struct or union that contains a member whose type is variable-length
  /// - An array with a variable-length element type
  /// - A typedef to a variable-length type
  ///
  /// Variable-length types have non-trivial constructor and destructor.
  static const bool is_var_len;

  /// `true` if the data type meets the CORBA Common Data Representation.
  ///
  /// is_CDR = `true` for all fixed-length data types, except for:
  /// - union
  /// - char
  /// - wchar
  /// - A struct or union that contains a member whose type is not CDR
  static const bool is_CDR;

  // Types for passing parameters via interface ABI
  typedef const ABI* ABI_in;
  typedef ABI* ABI_out;
  typedef ABI ABI_ret;
  typedef const ABI* ABI_VT_ret; // Valuetype attributes returned by const reference

  // Client-side types
  class C_in;
  class C_out;
  class C_inout;
  class C_ret;
  typedef C_VT_ret; // Used in Value Types to return state member values.

  // C_in, C_out and C_inout types cast to corresponding ABI types by operator &.

  // Servant-side methods
  static ConstRef in (ABI_in p);
  static Var& out (ABI_out p);
  static Var& inout (ABI_out p);
  static ABI_ret ret (Var);
  static ABI_ret ret (); // Return default value on error
  static ABI_VT_ret VT_ret (ConstRef);
  static ABI_VT_ret VT_ret (); // Return default value on error

  // Type code
  static I_ptr <TypeCode> type_code ();

  /// \brief Copies input data to the marshaling buffer.
  /// Must be defined if is_CDR = false.
  ///
  /// \param src Source value.
  /// \param rq  IORequest interface.
  static void marshal_in (const Var& src, IORequest_ptr rq);

  /// \brief Copies input data array to the marshaling buffer.
  /// Must be defined if is_CDR = false.
  ///
  /// \param src   Source values.
  /// \param count Count of source values.
  /// \param rq    IORequest interface.
  static void marshal_in_a (const Var* src, size_t count, IORequest_ptr rq);

  /// \brief Moves output data to the marshaling buffer.
  /// Must be defined if is_CDR = false.
  ///
  /// \param src Source value. After marshalling, the value will be released.
  /// \param rq  IORequest interface.
  static void marshal_out (Var& src, IORequest_ptr rq);

  /// \brief Moves output data array to the marshaling buffer.
  /// Must be defined if is_CDR = false.
  ///
  /// \param src   Source values. After marshalling, the values will be released.
  /// \param count Count of source values.
  /// \param rq    IORequest interface.
  static void marshal_out_a (Var* src, size_t count, IORequest_ptr rq);

  /// \brief Moves data from marshaling buffer to the local variable.
  /// Must be defined if is_CDR = false.
  ///
  /// \param rq    IORequest interface.
  /// \param dst   Destination value.
  static void unmarshal (IORequest_ptr rq, Var& dst);

  /// \brief Moves data array from marshaling buffer to the local variables.
  /// Must be defined if is_CDR = false.
  ///
  /// \param rq    IORequest interface.
  /// \param count Count of values.
  /// \param dst   Destination values buffer.
  static void unmarshal_a (IORequest_ptr rq, size_t count, Var* dst);

  /// \brief Swap byte order to change endianness.
  /// Must be defined if is_CDR = true.
  ///
  /// \param val Value.
  static void byteswap (Var&);
};
~~~
*/
template <class T> struct Type;

/// \brief Default base for IDL::traits<>
/// 
/// \typeparam T Type.
/// 
template <class T>
struct Traits
{
  using value_type = T;
  using in_type = const T&;
  using out_type = T&;
  using inout_type = T&;
};

}
}

namespace IDL {

/// Nirvana IDL type
template <class T>
using Type = CORBA::Internal::Type <T>;


/// Standard IDL traits
template <class> struct traits;

}

#endif
