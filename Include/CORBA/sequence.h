// Nirvana project
// Sequence - CORBA sequence declarations

#ifndef NIRVANA_ORB_SEQUENCE_H_
#define NIRVANA_ORB_SEQUENCE_H_

#include <Nirvana/vector.h>
#include "ABI.h"

namespace CORBA {
namespace Nirvana {

template <class T>
using Sequence = std::vector <T, std::allocator <T> >;

template <class T>
struct ABI <Sequence <T> > :
	public ABI_Variable <Sequence <T> >
{
	static void check (const Sequence <T>& v);
};

template <class T>
void ABI <std::vector <T, std::allocator <T> > >::check (const Sequence <T>& v)
{
	// Do some check
	const T* p = v.data ();
	if (p)
		CORBA::Nirvana::_check_pointer (p);
	size_t cnt = v.size ();
	if (cnt > 0 && (cnt > v.capacity () || !Sequence <T>::heap ()->is_readable (p, cnt * sizeof (T))))
		::Nirvana::throw_BAD_PARAM ();

	if (ABI <T>::is_checked)
		for (const T* end = p + cnt; p != end; ++p)
			ABI <T>::check (*p);
}

template <typename T>
using ABI_Sequence = ABI <Sequence <T> >;

template <typename T>
using Sequence_in = typename ABI_Sequence <T>::In;

template <typename T>
using Sequence_out = typename ABI_Sequence <T>::Out;

template <typename T>
using Sequence_inout = typename ABI_Sequence <T>::InOut;

template <typename T>
using Sequence_var = typename ABI_Sequence <T>::Var;

}
} // namespace CORBA

#endif
