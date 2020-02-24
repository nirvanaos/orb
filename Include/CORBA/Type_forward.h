/// IDL data type templates
#ifndef NIRVANA_ORB_TYPE_FORWARD_H_
#define NIRVANA_ORB_TYPE_FORWARD_H_

namespace CORBA {
namespace Nirvana {

/// For each structure, union or enum data type T, IDL compiler generates CORBA::Nirvana::ABI structure.
/// ABI type must be POD (mustn't have any constructors and destructors).
/// Compiler replaces all non-POD struct members with corresponding ABI structures.
template <class T> struct ABI;

/// For each structure, union or enum data type T, IDL compiler generates CORBA::Nirvana::Type structure:
///
///     template <> struct Type <T>
///     {
///       typedef ABI <T> ABI_type;
///
///       // Check internal invariants and throw BAD_PARAM or INV_OBJREF exception if data is invalid.
///       static void check (const ABI_type&);
///       
///       // true if check () method is not empty.
///       static const bool has_check;
///
///       // Types for passing parameters via interface ABI
///       typedef const ABI_type* ABI_in;
///       typedef ABI_type* ABI_out;
///       typedef ABI_type* ABI_inout;
///       typedef ABI_type ABI_ret;
///       typedef const ABI_type* ABI_VT_ret; // Valuetype attributes returned by const reference
///
///       // Client-side types
///       class C_var;
///       class C_in;
///       class C_out;
///       class C_inout;
///       class C_ret;
///       typedef C_VT_ret;
///
///       // C_in, C_out and C_inout types cast to corresponding ABI types by operator &.
///
///       // Servant-side methods
///       static <Servant in type> in (ABI_in p);
///       static <Servant out type> out (ABI_out p);
///       static <Servant inout type> inout (ABI_inout p);
///       static ABI_ret ret (<Servant return type>);
///       static ABI_VT_ret VT_ret (<Valuetype servant return type>);
///
///       // Servant types may be differ from client types.
///     };
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
