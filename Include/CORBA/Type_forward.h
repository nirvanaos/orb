/// IDL data type templates
#ifndef NIRVANA_ORB_TYPE_FORWARD_H_
#define NIRVANA_ORB_TYPE_FORWARD_H_

namespace CORBA {
namespace Nirvana {

template <class T> struct Type;

/// For each structure, union or enum data type T, IDL compiler generates CORBA::Nirvana::Type structure:
///
///     template <> struct Type <T>
///     {
///       // Check internal invariants and throw BAD_PARAM exception if data is invalid.
///       static void check (const T&);
///       
///       // true if check () method is not empty.
///       static const bool has_check;
///
///       // ABI types:
///       typedef ABI_in;
///       typedef ABI_out;
///       typedef ABI_inout;
///       typedef ABI_ret;
///
///       // Client types:
///       typedef C_in;
///       typedef C_out;
///       typedef C_inout;
///       typedef C_ret;
///       typedef C_var;
///
///       // Client type cast to corresponding ABI type by operator &
///
///       // Servant-side methods
///       static <type> in (ABI_in p);
///       static <type> out (ABI_out p);
///       static <type> inout (ABI_inout p);
///     };

// Helper functions.

extern void _check_pointer (const void* p);
extern bool uncaught_exception ();

template <class T>
using ABI_in = typename Type <T>::ABI_in;

template <class T>
using ABI_out = typename Type <T>::ABI_out;

template <class T>
using ABI_inout = typename Type <T>::ABI_inout;

template <class T>
using ABI_ret = typename Type <T>::ABI_ret;

}
}

#endif
