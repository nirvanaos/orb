#ifndef NIRVANA_ORB_TYPEFIXLEN_H_
#define NIRVANA_ORB_TYPEFIXLEN_H_

#include <Nirvana/NirvanaBase.h>
#include "TypeByValue.h"
#include "TypeByRef.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

/// Fixed-length data types.
/// Passed by value if sizeof (T) <= sizeof (Nirvana::Word).
template <class T>
using TypeFixLen = typename std::conditional <sizeof (T) <= 2 * sizeof (::Nirvana::Word), TypeByValue <T>, TypeByRef <T> >::type;

}
}

#endif
