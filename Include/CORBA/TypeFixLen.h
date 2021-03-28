#ifndef NIRVANA_ORB_TYPEFIXLEN_H_
#define NIRVANA_ORB_TYPEFIXLEN_H_

#include <Nirvana/NirvanaBase.h>
#include "TypeByVal.h"
#include "TypeByRef.h"
#include <type_traits>

namespace CORBA {
namespace Nirvana {

/// Fixed-length data types.
/// Passed by value if sizeof (T) <= 2 * sizeof (size_t).
template <class T
#ifdef NIRVANA_C11
	, typename Enable = void
#endif
>
using TypeFixLen = typename std::conditional <sizeof (T) <= 2 * sizeof (size_t), TypeByVal <T>, TypeByRef <T, T> >::type;

template <class T> struct Type :
	TypeFixLen <T
#ifdef NIRVANA_C11
	, typename std::enable_if <std::is_trivially_copyable <T>::value>::type
#endif
	>
{};

}
}

#endif
