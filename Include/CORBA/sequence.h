// Nirvana project
// Sequence - CORBA sequence declarations

#ifndef NIRVANA_ORB_SEQUENCE_H_
#define NIRVANA_ORB_SEQUENCE_H_

#include <Nirvana/vector.h>
#include "TypeVarLen.h"

namespace CORBA {
namespace Nirvana {

template <class T>
using Sequence = std::vector <T, std::allocator <T> >;

template <class T>
struct Type <Sequence <T> > :
	public TypeVarLen <Sequence <T> >
{
	static void check (const Sequence <T>& v)
	{
		// Do some check
		const T* p = v.data ();
		if (p)
			CORBA::Nirvana::_check_pointer (p);
		size_t cnt = v.size ();
		if (cnt > 0 && (cnt > v.capacity () || !Sequence <T>::memory ()->is_readable (p, cnt * sizeof (T))))
			::Nirvana::throw_BAD_PARAM ();

		if (Type <T>::has_check)
			for (const T* end = p + cnt; p != end; ++p)
				Type <T>::check (*p);
	}
};

template <>
struct Type <Sequence <bool> > :
	public Type <Sequence <Boolean> >
{};

template <typename T>
using TypeSequence = Type <Sequence <T> >;

template <typename T>
using Sequence_in = typename TypeSequence <T>::C_in;

template <typename T>
using Sequence_out = typename TypeSequence <T>::C_out;

template <typename T>
using Sequence_inout = typename TypeSequence <T>::C_inout;

template <typename T>
using Sequence_var = typename TypeSequence <T>::C_var;

}
} // namespace CORBA

#endif
