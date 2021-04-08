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

#include "primitive_types.h"

namespace CORBA {
namespace Nirvana {

/// We can not use `bool' built-in type across the binary boundaries because
/// it is compiler-specific, but we have to achieve the binary compatibility.
/// So we use size_t (the machine word) as ABI for boolean in assumption that bool implementation can't be wide.
/// Note that Sequence <bool> is implemented as vector <bool> template specialization
/// where element size is 1 byte.
typedef size_t ABI_boolean;

typedef ULong ABI_enum;

/// For each structure, union or enum data type T, IDL compiler generates `CORBA::Nirvana::ABI <T>` structure.
/// ABI type must be POD (Plain Old Data, mustn't have any constructors and destructors).
/// Compiler replaces all non-POD struct members with corresponding ABI structures.
template <class T> struct ABI;

/** For each structure, union or enum data type T, IDL compiler generates `CORBA::Nirvana::Type <T>` structure.
    This structure defines how the parameters are passed between client and server.
~~~
template <> struct Type <T>
{
  typedef T Var_type;
  typedef ABI <T> ABI_type;

  // Check internal invariants and throw BAD_PARAM or INV_OBJREF exception if data is invalid.
  static void check (const ABI_type&);

  // true if check () method is not empty.
  static const bool has_check;

  // Types for passing parameters via interface ABI
  typedef const ABI_type* ABI_in;
  typedef ABI_type* ABI_out;
  typedef ABI_type* ABI_inout;
  typedef ABI_type ABI_ret;
  typedef const ABI_type* ABI_VT_ret; // Valuetype attributes returned by const reference

  // Client-side types
  class C_var;
  class C_in;
  class C_out;
  class C_inout;
  class C_ret;
  typedef C_VT_ret;

  // C_in, C_out and C_inout types cast to corresponding ABI types by operator &.

  // Servant-side methods
  static <Servant in type> in (ABI_in p);
  static <Servant out type> out (ABI_out p);
  static <Servant inout type> inout (ABI_inout p);
  static ABI_ret ret (<Servant return type>);
  static ABI_VT_ret VT_ret (<Valuetype servant return type>);

  // NOTE: Servant types may be differ from client types.

  // Types for members
  typedef T Member_type;
  typedef const T& Member_ret;

  // Type code
  static TypeCode_ptr type_code ();
};
~~~
*/
template <class T> struct Type;

template <class T>
using ABI_in = typename Type <T>::ABI_in;

template <class T>
using ABI_out = typename Type <T>::ABI_out;

template <class T>
using ABI_inout = typename Type <T>::ABI_inout;

template <class T>
using ABI_ret = typename Type <T>::ABI_ret;

template <class T>
using ABI_VT_ret = typename Type <T>::ABI_VT_ret;

template <class T>
using T_in = typename Type <T>::C_in;

template <class T>
using T_out = typename Type <T>::C_out;

template <class T>
using T_inout = typename Type <T>::C_inout;

template <class T>
using T_ret = typename Type <T>::C_ret;

template <class T>
using T_VT_ret = typename Type <T>::C_VT_ret;

// Constraints check
const bool CHECK_STRINGS = true;
const bool CHECK_SEQUENCES = true;

// Helper functions.
extern void _check_pointer (const void* p);
extern bool uncaught_exception ();

}
}

#endif
